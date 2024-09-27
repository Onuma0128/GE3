#include "Object3d.h"
#include "Object3dBase.h"
#include "LightManager.h"

void Object3d::Initialize()
{
	this->object3dBase_ = Object3dBase::GetInstance();

    MakeWvpData();

    transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void Object3d::Update()
{
    Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
    Matrix4x4 worldMatrixObject = MakeAfineMatrix(transform_.scale, transform_.rotate, transform_.translate);
    Matrix4x4 worldViewMatrixObject = Multiply(worldMatrixObject, object3dBase_->GetDxEngine()->GetCameraView()); // カメラから見たワールド座標に変換
    Matrix4x4 worldViewProjectionMatrixObject = Multiply(worldViewMatrixObject, projectionMatrix); // 射影行列を適用してワールドビュープロジェクション行列を計算
    wvpData_->WVP = worldViewProjectionMatrixObject; // ワールドビュープロジェクション行列を更新
    wvpData_->World = worldViewMatrixObject; // ワールド座標行列を更新
    wvpData_->WorldInverseTranspose = Inverse(worldViewMatrixObject);
}

void Object3d::Draw()
{
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(3, LightManager::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(4, LightManager::GetInstance()->GetPointLightResource()->GetGPUVirtualAddress());
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(5, LightManager::GetInstance()->GetSpotLightResource()->GetGPUVirtualAddress());
    object3dBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(6, object3dBase_->GetDxEngine()->GetCameraResource()->GetGPUVirtualAddress());

    if (model_) {
        model_->Draw();
    }

}

void Object3d::MakeWvpData()
{
    wvpResource_ = CreateBufferResource(object3dBase_->GetDxEngine()->GetDevice(), sizeof(Matrix4x4)).Get();
    wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
    wvpData_->WVP = MakeIdentity4x4();
    wvpData_->World = MakeIdentity4x4();
}

