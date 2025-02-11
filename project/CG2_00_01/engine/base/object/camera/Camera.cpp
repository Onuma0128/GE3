#include "Camera.h"

#include "WinApp.h"
#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

void Camera::Initialize()
{
	input_ = Input::GetInstance();

	debugTransform_ = { {1.0f,1.0f,1.0f},{0.26f,0.0f,0.0f},{0.0f,4.0f,-15.0f} };
	transform_ = { {1.0f,1.0f,1.0f},{0.26f,0.0f,0.0f},{0.0f,4.0f,-15.0f} };
	fovY_ = 0.45f;
	aspectRatio_ = float(WinApp::kClientWidth) / float(WinApp::kClientHeight);
	nearClip_ = 0.1f;
	farClip_ = 100.0f;

	UpdateMatrix(transform_);
	isDebug_ = false;
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

void Camera::DebugCamera()
{
	const float moveSpeed = 0.1f;

	Vector3 defaultForward = Vector3::ExprUnitZ;
	Matrix4x4 rotationMatrix = Matrix4x4::RotateY(debugTransform_.rotation.y);
	Vector3 forward = defaultForward.Transform(rotationMatrix);

	// 右方向ベクトルを計算
	Vector3 defaultRight = Vector3::ExprUnitX;
	Vector3 right = defaultRight.Transform(rotationMatrix);

	if (input_->PushKey(DIK_W)) {
		debugTransform_.translation += forward * moveSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		debugTransform_.translation -= forward * moveSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		debugTransform_.translation -= right * moveSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		debugTransform_.translation += right * moveSpeed;
	}
	if (input_->PushKey(DIK_Q)) {
		debugTransform_.translation.y -= moveSpeed;
	}
	if (input_->PushKey(DIK_E)) {
		debugTransform_.translation.y += moveSpeed;
	}

	// 右クリックされているなら
	if (input_->PushMouseButton(1)) {
		// マウスによるカメラ回転
		float mouseDeltaX = static_cast<float>(input_->GetMouseDeltaX());
		float mouseDeltaY = static_cast<float>(input_->GetMouseDeltaY());

		// マウスの移動をカメラの回転に変換
		debugTransform_.rotation.y += mouseDeltaX * mouseSensitivity_;
		debugTransform_.rotation.x += mouseDeltaY * mouseSensitivity_;
	}

	UpdateMatrix(debugTransform_);
}

void Camera::NormalCamera()
{
	debugTransform_ = transform_;
	UpdateMatrix(transform_);
}

void Camera::UpdateMatrix(Transform transform)
{
	worldMatrix_ = Matrix4x4::Affine(transform.scale, transform.rotation, transform.translation);

	viewMatrix_ = Matrix4x4::Inverse(worldMatrix_);

	projectionMatrix_ = Matrix4x4::PerspectiveFov(fovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;
}

void Camera::CameraImGui()
{
#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::Checkbox("debug", &isDebug_);

	Vector3 rotation = transform_.rotation;
	Vector3 translation = transform_.translation;
	ImGui::DragFloat3("rotate", &rotation.x, 0.01f);
	ImGui::DragFloat3("translate", &translation.x, 0.01f);
	transform_.rotation = rotation;
	transform_.translation = translation;

	ImGui::End();
#endif // _DEBUG
}