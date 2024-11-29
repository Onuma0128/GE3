#include "ParticleManager.h"

#include "PrimitiveDrawer.h"
#include "LightManager.h"
#include "TextureManager.h"
#include "Camera.h"

#include "CreateBufferResource.h"
#include "ParticleEmitter.h"

ParticleManager* ParticleManager::instance_ = nullptr;

ParticleManager* ParticleManager::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = new ParticleManager;
    }
    return instance_;
}

void ParticleManager::Initialize(DirectXEngine* dxEngine)
{
    dxEngine_ = dxEngine;
    srvManager_ = SrvManager::GetInstance();

    rootSignature_ = dxEngine_->GetPipelineState()->CreateParticleRootSignature();
    pipelineState_ = dxEngine_->GetPipelineState()->CreateParticlePipelineState();
    
    modelData_ = Model::LoadObjFile("resources", "plane.obj");

    CreateVertexResource();

    CreateMatrialResource();
}

void ParticleManager::Update()
{
    // 各パーティクルグループに対して処理
    for (auto& [name, group] : particleGroups_) {
        // 新しいパーティクルを追加する
        particleGroups_[name].emitter->Update();

        uint32_t numInstance = 0;
        // 各パーティクルを更新
        for (auto it = group.particles.begin(); it != group.particles.end();) {
            if (it->lifeTime <= it->currentTime) {
                it = group.particles.erase(it);
                continue;
            }

            // ビルボード前にパーティクルの回転を決める
            Matrix4x4 rotateX = Matrix4x4::RotateX(0);
            Matrix4x4 rotateY = Matrix4x4::RotateY(std::numbers::pi_v<float>);
            Matrix4x4 rotateZ = Matrix4x4::RotateZ(0);
            Matrix4x4 backToFrontMatrix = rotateX * rotateY * rotateZ;

            // パーティクルのビルボード化
            Matrix4x4 billboardMatrix = backToFrontMatrix * Camera::GetInstance()->GetWorldMatrix();
            billboardMatrix.m[3][0] = 0.0f;
            billboardMatrix.m[3][1] = 0.0f;
            billboardMatrix.m[3][2] = 0.0f;
            Matrix4x4 worldMatrix = Matrix4x4::Scale(it->transform.scale) * billboardMatrix * Matrix4x4::Translate(it->transform.translate);
            Matrix4x4 worldViewMatrix = worldMatrix * Camera::GetInstance()->GetViewMatrix();
            Matrix4x4 worldViewProjectionMatrix = worldViewMatrix * Camera::GetInstance()->GetProjectionMatrix();

            // パーティクルの更新
            group.emitter->UpdateParticle(it);

            float alpha = 1.0f - (it->currentTime / it->lifeTime);
            if (numInstance < kNumMaxInstance) {
                group.instancingData[numInstance].WVP = worldViewProjectionMatrix;
                group.instancingData[numInstance].World = worldViewMatrix;
                group.instancingData[numInstance].color = it->color;
                group.instancingData[numInstance].color.w = alpha;
                ++numInstance;
            }
            ++it;
        }

        group.instanceCount = numInstance;
        std::memcpy(group.instancingData, group.instancingData, sizeof(ParticleForGPU) * numInstance);
    }
}

void ParticleManager::Draw()
{
    /*==================== パイプラインの設定 ====================*/
    dxEngine_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
    dxEngine_->GetCommandList()->SetPipelineState(pipelineState_.Get());
    dxEngine_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    /*==================== パーティクルの描画 ====================*/
    for (const auto& [name, group] : particleGroups_) {
        uint32_t textIndex = group.textureIndex;
        dxEngine_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
        dxEngine_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
        dxEngine_->GetCommandList()->SetGraphicsRootConstantBufferView(1, group.instancingResource->GetGPUVirtualAddress());
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, textIndex);
        SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(3, group.srvIndex);

        // 各パーティクルグループのインスタンスを描画
        dxEngine_->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), group.instanceCount, 0, 0);
    }

    for (const auto& [name, group] : particleGroups_) {
        group.emitter->Draw();
    }
}

void ParticleManager::Finalize()
{
    delete instance_;
    instance_ = nullptr;
}

void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath)
{
    ParticleGroup group;
    group.textureFilePath = textureFilePath;
    TextureManager::GetInstance()->LoadTexture("resources/" + group.textureFilePath);
    group.textureIndex = TextureManager::GetInstance()->GetSrvIndex("resources/" + group.textureFilePath);

    // パーティクルグループのインスタンス数とリソースを初期化
    group.instanceCount = 0;
    group.instancingData = nullptr;

    // インスタンスデータ用のバッファリソースを作成
    group.instancingResource = CreateBufferResource(
        dxEngine_->GetDevice(),
        sizeof(ParticleForGPU) * kNumMaxInstance).Get();

    // リソースをマッピングして、インスタンスデータを初期化
    group.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&group.instancingData));
    for (uint32_t i = 0; i < kNumMaxInstance; ++i) {
        group.instancingData[i].WVP = Matrix4x4::Identity();
        group.instancingData[i].World = Matrix4x4::Identity();
        group.instancingData[i].color = Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
    }
    group.emitter = std::make_unique<ParticleEmitter>(name);

    group.srvIndex = srvManager_->Allocate() + TextureManager::kSRVIndexTop;

    srvManager_->CreateSRVforStructuredBuffer(
        group.srvIndex,
        group.instancingResource.Get(),
        kNumMaxInstance,
        sizeof(ParticleManager::ParticleForGPU)
    );

    // 新しいパーティクルグループを particleGroups_ マップに追加
    particleGroups_[name] = std::move(group);
}

void ParticleManager::Emit(const std::string name, const Vector3& position, uint32_t count)
{
    // 新しいパーティクルを追加する
    particleGroups_[name].emitter->CreateParticles(particleGroups_[name]);
    particleGroups_[name].emitter->SetEmitPosition(position);
    particleGroups_[name].emitter->SetEmitCount(count);
}

void ParticleManager::CreateVertexResource()
{
    // 実際に頂点リソースを作る
    vertexResource_ = CreateBufferResource(dxEngine_->GetDevice(), sizeof(VertexData) * modelData_.vertices.size()).Get();
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
    std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
}

void ParticleManager::CreateMatrialResource()
{
    // マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
    materialResource_ = CreateBufferResource(dxEngine_->GetDevice(), sizeof(Material)).Get();
    // 書き込むためのアドレスを取得
    materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
    // 今回は白を書き込んでいく
    materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData_->enableLighting = false;
    materialData_->uvTransform = Matrix4x4::Identity();
}
