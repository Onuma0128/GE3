#include "Camera.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include "WinApp.h"
#include "MT3.h"

Camera* Camera::instance_ = nullptr;

Camera* Camera::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new Camera;
	}
	return instance_;
}

void Camera::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	transform_ = { {1.0f,1.0f,1.0f},{0.26f,0.0f,0.0f},{0.0f,4.0f,-15.0f} };
	fovY_ = 0.45f;
	aspectRatio_ = float(WinApp::kClientWidth) / float(WinApp::kClientHeight);
	nearClip_ = 0.1f;
	farClip_ = 100.0f;

	worldMatrix_ = MakeAfineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;

	MakeCameraData();
}

void Camera::Update()
{
	worldMatrix_ = MakeAfineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Inverse(worldMatrix_);

	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;

	cameraData_->worldPosition = transform_.translate;
}

void Camera::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}

void Camera::MakeCameraData()
{
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	cameraResource_ = CreateBufferResource(dxEngine_->GetDevice(), sizeof(Vector3)).Get();

	// 書き込むためのアドレスを取得
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));

	// 単位行列を書き込んでおく
	cameraData_->worldPosition = transform_.translate;
}
