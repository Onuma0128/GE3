#include "RailCamera.h"
#include "imgui.h"
#include "cmath"

void RailCamera::Initialize()
{
	camera_ = Camera::GetInstance();

	cameraObj_ = std::make_unique<Object3d>();
	cameraObj_->Initialize();
	cameraObj_->SetModel("suzanne.obj");

	newLine_ = std::make_unique<Object3d>();
	newLine_->Initialize();
	newLine_->SetModel("teapot.obj");
	newLine_->SetScale({ 0.5f,0.5f,0.5f });

	controlPoints_ = {
		{0,0,0},
		{0,1,2},
		{0,2,4},
		{0,2,6},
		{-2,2,8},
		{-4,2,8},
		{-6,2,8},
	};

	// ラインの複数描画
	CreateRail();
	line3d_ = std::make_unique<Line3d>();
	line3d_->Initialize(lines);
	
	t_ = 0.0f;
}

void RailCamera::Update() 
{
	// レールカメラの動き
	RailCameraMove();

	// カメラのオブジェクト
	cameraObj_->Update();

	// 新しいライン生成オブジェクト
	newLine_->SetPosition(newLinePos_);
	newLine_->Update();

	// ラインの複数オブジェクト
	line3d_->Update();

	// レールの板ポリ
	for (auto& rail : railObj_) {
		rail->Update();
	}
}

void RailCamera::Draw()
{
	if (isDebug_) {
		cameraObj_->Draw();
	}

	newLine_->Draw();

	for (auto& rail : railObj_) {
		rail->Draw();
	}
}

void RailCamera::DrawLine()
{
	// ラインの複数描画
	line3d_->Draws();
}

void RailCamera::Debug_ImGui()
{
	ImGui::Begin("RailCamera");
	if (ImGui::Button("AddLine")) {
		t_ = 0.0f;
		controlPoints_.push_back(newLine_->GetPosition());
		lines.clear();
		railObj_.clear();
		
		CreateRail();

		line3d_->Initialize(lines);
	}
	ImGui::DragFloat3("position", &newLinePos_.x, 0.1f);
	ImGui::Checkbox("debugCamera", &isDebug_);
	if (ImGui::Button("cameraReset")) {
		camera_->SetRotate({ 0.26f,0.0f,0.0f });
		camera_->SetTranslate({ 0.0f,4.0f,-15.0f });
	}
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
		if (!isDebug_) {
			camera_->SetTranslate(position);
		}
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
		if (!isDebug_) {
			camera_->SetRotate(rotate);
		}
	}
}

void RailCamera::CreateRail()
{
	for (int i = 0; i < (int)lineNum; ++i) {
		if (!controlPoints_.empty()) {
			// スプライン上の現在の位置を計算
			Vector3 position = CatmullRomPosition(controlPoints_, t_);
			t_ += 1.0f / lineNum;
			Vector3 nextPosition = CatmullRomPosition(controlPoints_, t_);
			lines.push_back(position);
			lines.push_back(nextPosition);

			// 回転の計算
			Vector3 velocity = Subtract(nextPosition, position);
			Vector3 rotate{};
			rotate.y = std::atan2(velocity.x, velocity.z);
			Matrix4x4 rotateMatrixY = MakeRotateYMatrix(-rotate.y);
			Vector3 velocityZ = Transform_(velocity, rotateMatrixY);
			rotate.x = std::atan2(-velocityZ.y, velocityZ.z);

			std::unique_ptr<Object3d> rail = std::make_unique<Object3d>();
			rail->Initialize();
			rail->SetModel("railPlane.obj");
			rail->SetRotation(rotate);
			rail->SetPosition(position);
			railObj_.push_back(std::move(rail));
		}
	}
}
