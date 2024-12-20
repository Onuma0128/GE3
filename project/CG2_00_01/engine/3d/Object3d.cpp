#include "Object3d.h"

#include "CameraManager.h"
#include "Object3dBase.h"
#include "LightManager.h"
#include "ModelManager.h"

#include "WorldTransform.h"

#include "CreateBufferResource.h"

void Object3d::Initialize(const std::string& filePath, WorldTransform* transform)
{
    this->object3dBase_ = Object3dBase::GetInstance();

    transform_ = transform;

    SetModel(filePath);

    MakeMaterialData();
}

void Object3d::Update()
{
    transform_->TransferMatrix(model_);
}

void Object3d::Draw()
{
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transform_->GetConstBuffer()->GetGPUVirtualAddress());
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(3, LightManager::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(4, LightManager::GetInstance()->GetPointLightResource()->GetGPUVirtualAddress());
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(5, LightManager::GetInstance()->GetSpotLightResource()->GetGPUVirtualAddress());
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(6, CameraManager::GetInstance()->GetCameraResource()->GetGPUVirtualAddress());

    if (model_) {
        model_->Draw();
    }

}

void Object3d::SetModel(const std::string& filePath)
{
    model_ = ModelManager::GetInstance()->FindModel(filePath);
}

void Object3d::SetTexture(const std::string& directoryPath, const std::string& filePath)
{
    model_->SetTexture(directoryPath, filePath);
}

void Object3d::SetColor(const Vector4& color)
{
    materialData_->color = color;
}

void Object3d::MakeMaterialData()
{
    // マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
    materialResource_ = CreateBufferResource(object3dBase_->GetDxEngine()->GetDevice(), sizeof(Material)).Get();
    // 書き込むためのアドレスを取得
    materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
    // 今回は白を書き込んでいく
    materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData_->enableLighting = true;
    materialData_->uvTransform = Matrix4x4::Identity();
    materialData_->shininess = 20.0f;
}