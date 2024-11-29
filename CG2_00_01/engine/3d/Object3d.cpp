#include "Object3d.h"

#include "Object3dBase.h"
#include "LightManager.h"
#include "ModelManager.h"
#include "CreateBufferResource.h"

void Object3d::Initialize(const std::string& filePath)
{
    this->object3dBase_ = Object3dBase::GetInstance();
    this->camera_ = Object3dBase::GetInstance()->GetDefaultCamera();

    SetModel(filePath);

    MakeWvpData();

    transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void Object3d::Update()
{
    worldMatrix_ = Matrix4x4::Affine(transform_.scale, transform_.rotate, transform_.translate);
    if (parent_) {
        worldMatrix_ = worldMatrix_ * parent_->GetWorldMatrix();
    }
    Matrix4x4 worldViewMatrixObject = worldMatrix_ * camera_->GetViewMatrix(); // カメラから見たワールド座標に変換
    Matrix4x4 worldViewProjectionMatrixObject = worldViewMatrixObject * camera_->GetProjectionMatrix(); // 射影行列を適用してワールドビュープロジェクション行列を計算
    wvpData_->WVP = model_->GetModelData().rootNode.localMatrix * worldViewProjectionMatrixObject; // ワールドビュープロジェクション行列を更新
    wvpData_->World = model_->GetModelData().rootNode.localMatrix * worldViewMatrixObject; // ワールド座標行列を更新
    wvpData_->WorldInverseTranspose = model_->GetModelData().rootNode.localMatrix * Matrix4x4::Inverse(worldViewMatrixObject);
}

void Object3d::Draw()
{
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(3, LightManager::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(4, LightManager::GetInstance()->GetPointLightResource()->GetGPUVirtualAddress());
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(5, LightManager::GetInstance()->GetSpotLightResource()->GetGPUVirtualAddress());
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(6, camera_->GetCameraResource()->GetGPUVirtualAddress());

    if (model_) {
        model_->Draw();
    }

}

void Object3d::MakeWvpData()
{
    wvpResource_ = CreateBufferResource(object3dBase_->GetDxEngine()->GetDevice(), sizeof(Matrix4x4)).Get();
    wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
    wvpData_->WVP = Matrix4x4::Identity();
    wvpData_->World = Matrix4x4::Identity();
}

void Object3d::SetModel(const std::string& filePath)
{
    model_ = ModelManager::GetInstance()->FindModel(filePath);
}

const Vector3 Object3d::GetWorldPosition()
{
    Vector3 translate = { worldMatrix_.m[3][0],worldMatrix_.m[3][1],worldMatrix_.m[3][2] };
    return translate;
}

const Matrix4x4& Object3d::GetWorldMatrix()
{
    return worldMatrix_;
}