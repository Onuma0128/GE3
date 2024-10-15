#include "Camera.h"

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

	input_ = Input::GetInstance();

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
#ifdef _DEBUG
	DebugCamera();
#endif // _DEBUG

	worldMatrix_ = MakeAfineMatrix(transform_.scale, transform_.rotate, transform_.translate);

	Vector3 eye = transform_.translate;
	Vector3 defaultForward = { 0.0f, 0.0f, 1.0f };
	Matrix4x4 rotationMatrix = MakeRotateMatrix(transform_.rotate);
	Vector3 forward = Transform_(defaultForward,rotationMatrix);
	Vector3 target = eye + forward;
	viewMatrix_ = LookAt(eye, target, { 0.0f, 1.0f, 0.0f });

	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;

	cameraData_->worldPosition = transform_.translate;
}

void Camera::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}

void Camera::DebugCamera()
{
	const float moveSpeed = 0.1f;

	Vector3 defaultForward = { 0.0f, 0.0f, 1.0f };
	Matrix4x4 rotationMatrix = MakeRotateYMatrix(transform_.rotate.y);
	Vector3 forward = Transform_(defaultForward, rotationMatrix);

	// 右方向ベクトルを計算
	Vector3 defaultRight = { 1.0f, 0.0f, 0.0f };
	Vector3 right = Transform_(defaultRight, rotationMatrix);

	if (input_->PushKey(DIK_W)) {
		transform_.translate = transform_.translate + forward * moveSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		transform_.translate = transform_.translate - forward * moveSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		transform_.translate = transform_.translate - right * moveSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		transform_.translate = transform_.translate + right * moveSpeed;
	}

	// 右クリックされているなら
	if (input_->PushMouseButton(1)) {
		// マウスによるカメラ回転
		float mouseDeltaX = static_cast<float>(input_->GetMouseDeltaX());
		float mouseDeltaY = static_cast<float>(input_->GetMouseDeltaY());

		// マウスの移動をカメラの回転に変換
		transform_.rotate.y -= mouseDeltaX * mouseSensitivity_;
		transform_.rotate.x -= mouseDeltaY * mouseSensitivity_;
	}
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
