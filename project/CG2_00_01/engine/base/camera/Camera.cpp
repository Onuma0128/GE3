#include "Camera.h"

#include "WinApp.h"
#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

#include "CreateBufferResource.h"

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

	debugTransform_ = { {1.0f,1.0f,1.0f},{0.26f,0.0f,0.0f},{0.0f,4.0f,-15.0f} };
	transform_ = { {1.0f,1.0f,1.0f},{0.26f,0.0f,0.0f},{0.0f,4.0f,-15.0f} };
	fovY_ = 0.45f;
	aspectRatio_ = float(WinApp::kClientWidth) / float(WinApp::kClientHeight);
	nearClip_ = 0.1f;
	farClip_ = 100.0f;

	worldMatrix_ = Matrix4x4::Affine(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(worldMatrix_);
	projectionMatrix_ = Matrix4x4::PerspectiveFov(fovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;

	MakeCameraData();
}

void Camera::Update()
{
#ifdef _DEBUG
	if (isDebug_) {
		DebugCamera();
		return;
	}
#endif // _DEBUG
	NormalCamera();
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
	Matrix4x4 rotationMatrix = Matrix4x4::RotateY(debugTransform_.rotate.y);
	Vector3 forward = defaultForward.Transform(rotationMatrix);

	// 右方向ベクトルを計算
	Vector3 defaultRight = { 1.0f, 0.0f, 0.0f };
	Vector3 right = defaultRight.Transform(rotationMatrix);

	if (input_->PushKey(DIK_W)) {
		debugTransform_.translate = debugTransform_.translate + forward * moveSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		debugTransform_.translate = debugTransform_.translate - forward * moveSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		debugTransform_.translate = debugTransform_.translate - right * moveSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		debugTransform_.translate = debugTransform_.translate + right * moveSpeed;
	}
	if (input_->PushKey(DIK_Q)) {
		debugTransform_.translate.y -= moveSpeed;
	}
	if (input_->PushKey(DIK_E)) {
		debugTransform_.translate.y += moveSpeed;
	}

	// 右クリックされているなら
	if (input_->PushMouseButton(1)) {
		// マウスによるカメラ回転
		float mouseDeltaX = static_cast<float>(input_->GetMouseDeltaX());
		float mouseDeltaY = static_cast<float>(input_->GetMouseDeltaY());

		// マウスの移動をカメラの回転に変換
		debugTransform_.rotate.y -= mouseDeltaX * mouseSensitivity_;
		debugTransform_.rotate.x -= mouseDeltaY * mouseSensitivity_;
	}

	UpdateMatrix(debugTransform_);

	// カメラのデータを更新
	cameraData_->worldPosition = debugTransform_.translate;
}

void Camera::NormalCamera()
{
	UpdateMatrix(transform_);

	// カメラのデータを更新
	cameraData_->worldPosition = transform_.translate;
}

void Camera::UpdateMatrix(Transform transform)
{
	Matrix4x4 rotationMatrix = Matrix4x4::Rotate(transform.rotate);
	worldMatrix_ = Matrix4x4::Affine(transform.scale, transform.rotate, transform.translate);

	viewMatrix_ = Matrix4x4::Inverse(worldMatrix_);

	projectionMatrix_ = Matrix4x4::PerspectiveFov(fovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;
}

void Camera::CameraImGui()
{
#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::Checkbox("debug", &isDebug_);

	rotate_ = transform_.rotate;
	translate_ = transform_.translate;
	ImGui::DragFloat3("rotate", &rotate_.x,0.01f);
	ImGui::DragFloat3("translate", &translate_.x,0.01f);
	transform_.rotate = rotate_;
	transform_.translate = translate_;

	ImGui::End();
#endif // _DEBUG
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