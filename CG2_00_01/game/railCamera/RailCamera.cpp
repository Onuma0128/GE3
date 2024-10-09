#include "RailCamera.h"
#include "imgui.h"

void RailCamera::Initialize()
{
	camera_ = Camera::GetInstance();
	input_ = Input::GetInstance();
	cameraObj_ = std::make_unique<Object3d>();
	cameraObj_->Initialize();
	cameraObj_->SetModel("suzanne.obj");

	cameraOffset_ = { {1,1,1},{0,0,0},{0,0,0} };

	controlPoints_ = {
		{0,0,0},
		{0,3,5},
		{0,5,10},
		{0,3,15},
		{0,0,20}
	};

	for (uint32_t i = 0; i < controlPoints_.size(); ++i) {
		std::unique_ptr<Object3d> obj = std::make_unique<Object3d>();
		obj->Initialize();
		obj->SetModel("teapot.obj");
		obj->SetPosition(controlPoints_[i]);

		railPoints_.push_back(std::move(obj));
	}
}

void RailCamera::Update() 
{
	cameraObj_->Update();

	camera_->SetRotate(cameraOffset_.rotate);
	camera_->SetTranslate(cameraOffset_.translate);

	RailCameraMove();

	for (auto& obj : railPoints_) {
		obj->Update();
	}
}

void RailCamera::Draw()
{
	cameraObj_->Draw();

	for (auto& obj : railPoints_) {
		obj->Draw();
	}
}

void RailCamera::Debug_ImGui()
{
	ImGui::Begin("camera");
	ImGui::DragFloat3("rotate", &cameraOffset_.rotate.x, 0.01f);
	ImGui::DragFloat3("translate", &cameraOffset_.translate.x, 0.1f);
	ImGui::End();
}

void RailCamera::RailCameraMove()
{
	// 時間の進行量
	const float dt = 0.001f;
	if (!controlPoints_.empty()) {
		// スプライン上の現在の位置を計算
		Vector3 position = CatmullRomPosition(controlPoints_, t_);
		// 少し先の位置を計算し、注視点を決定
		float nextT = t_ + dt;
		Vector3 lookAtPosition = CatmullRomPosition(controlPoints_, nextT);
		// カメラの位置を更新
		cameraObj_->SetPosition(position);
		// 時間を進行
		t_ += dt;
		if (t_ > 1.0f) {
			t_ = 0.0f; // ループさせる場合
		}
		Vector3 velocity = Subtract(lookAtPosition, position);

		Vector3 rotate{};
		rotate.y = std::atan2(velocity.x, velocity.z);
		Matrix4x4 rotateMatrixY = MakeRotateYMatrix(-rotate.y);
		Vector3 velocityZ = Transform_(velocity, rotateMatrixY);
		rotate.x = std::atan2(-velocityZ.y, velocityZ.z);
		cameraObj_->SetRotation(rotate);
	}
}
