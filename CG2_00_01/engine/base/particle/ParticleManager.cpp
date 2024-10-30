#include "ParticleManager.h"

#include "LightManager.h"
#include "TextureManager.h"
#include "Camera.h"

#include "CreateBufferResource.h"

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

    CreateInstancingResource();

    CreateEmit();
}

void ParticleManager::Update()
{
    // エミッターによるパーティクルの発生
    if (moveStart_) {
        emitter_.frequencyTime += kDeltaTime;
    }
    if (emitter_.frequency <= emitter_.frequencyTime) {
        std::mt19937 randomEngine_(seedGenerator_());
        particles_.splice(particles_.end(), Emit(emitter_, randomEngine_));
        emitter_.frequencyTime -= emitter_.frequency;
    }

    // パーティクルの更新
    uint32_t numInstance = 0;
    for (std::list<Particle>::iterator particleIterator = particles_.begin();
        particleIterator != particles_.end();) {
        if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
            particleIterator = particles_.erase(particleIterator);
            continue;
        }
        Matrix4x4 backToFrontMatrix = Matrix4x4::RotateY(std::numbers::pi_v<float>);
        Matrix4x4 billboardMatrix = backToFrontMatrix * Camera::GetInstance()->GetWorldMatrix();
        billboardMatrix.m[3][0] = 0.0f; billboardMatrix.m[3][1] = 0.0f; billboardMatrix.m[3][2] = 0.0f;
        Matrix4x4 worldMatrix = Matrix4x4::Scale(particleIterator->transform.scale) * billboardMatrix * Matrix4x4::Translate(particleIterator->transform.translate);
        Matrix4x4 worldViewMatrix = worldMatrix * Camera::GetInstance()->GetViewMatrix();
        Matrix4x4 worldViewProjectionMatrix = worldViewMatrix * Camera::GetInstance()->GetProjectionMatrix();
        //パーティクルの動き
        if (moveStart_) {
            if (IsCollision(accelerationField_.area, particleIterator->transform.translate) && isFieldStart_) {
                particleIterator->velocity = accelerationField_.acceleration * kDeltaTime + particleIterator->velocity;
            }
            particleIterator->transform.translate = particleIterator->velocity * kDeltaTime + particleIterator->transform.translate;
            particleIterator->currentTime += kDeltaTime;
        }
        float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime);
        if (numInstance < kNumMaxInstance) {
            instancingData_[numInstance].WVP = worldViewProjectionMatrix;
            instancingData_[numInstance].World = worldViewMatrix;
            instancingData_[numInstance].color = particleIterator->color;
            instancingData_[numInstance].color.w = alpha;
            ++numInstance;
        }
        ++particleIterator;
    }
}

void ParticleManager::Draw()
{
    /*==================== パイプラインの設定 ====================*/
    dxEngine_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
    dxEngine_->GetCommandList()->SetPipelineState(pipelineState_.Get());
    dxEngine_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Particle
    TextureManager::GetInstance()->LoadTexture("resources/circle.png");
    uint32_t textIndex = TextureManager::GetInstance()->GetSrvIndex("resources/uvChecker.png");
    dxEngine_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
    dxEngine_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    dxEngine_->GetCommandList()->SetGraphicsRootConstantBufferView(1, instancingResource_->GetGPUVirtualAddress());
    SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, textIndex);
    dxEngine_->GetCommandList()->SetGraphicsRootConstantBufferView(3, LightManager::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
    SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(4, 1);
    // 描画
    dxEngine_->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), kNumMaxInstance, 0, 0);
}

void ParticleManager::Finalize()
{
    delete instance_;
    instance_ = nullptr;
}

void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath)
{

}

std::list<ParticleManager::Particle> ParticleManager::Emit(const Emitter& emitter, std::mt19937& randomEngine)
{
    std::list<Particle> particles;
    for (uint32_t count = 0; count < emitter.count; ++count) {
        particles.push_back(MakeNewParticle(randomEngine, emitter.transform.translate));
    }
    return particles;
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

void ParticleManager::CreateInstancingResource()
{
    // Instancing用
    instancingResource_ = CreateBufferResource(dxEngine_->GetDevice(), sizeof(ParticleForGPU) * kNumMaxInstance).Get();
    instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
    for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
        instancingData_[index].WVP = Matrix4x4::Identity();
        instancingData_[index].World = Matrix4x4::Identity();
        instancingData_[index].color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
    }
}

void ParticleManager::CreateEmit()
{
    std::mt19937 randomEngine_(seedGenerator_());

    emitter_.transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
    emitter_.frequency = 0.5f;
    emitter_.frequencyTime = 0.0f;
    accelerationField_.acceleration = { 0.0f,10.0f,0.0f };
    accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
    accelerationField_.area.max = { 1.0f,1.0f,1.0f };
    // emitter_.frequencyごとに出すパーティクルの個数
    emitter_.count = 3;
    particles_.splice(particles_.end(), Emit(emitter_, randomEngine_));
    moveStart_ = true;
    isFieldStart_ = false;
}

ParticleManager::Particle ParticleManager::MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate)
{
    std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
    std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
    std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
    Particle particle{};
    particle.transform.scale = { 1.0f,1.0f,1.0f };
    particle.transform.rotate = { 0.0f,0.0f,0.0f };
    Vector3 randomTranslate = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };
    particle.transform.translate = translate + randomTranslate;
    particle.velocity = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };
    particle.color = { distColor(randomEngine),distColor(randomEngine) ,distColor(randomEngine) ,1.0f };
    particle.lifeTime = distTime(randomEngine);
    particle.currentTime = 0.0f;
    return particle;
}

bool ParticleManager::IsCollision(const AABB& aabb, const Vector3& point)
{
    if (aabb.min.x < point.x && aabb.max.x > point.x &&
        aabb.min.y < point.y && aabb.max.y > point.y &&
        aabb.min.z < point.z && aabb.max.z > point.z) {
        return true;
    }
    return false;
}
