#include "RailCamera.h"
#include "imgui.h"
#include "cmath"

void RailCamera::Initialize()
{
	cameraObj_ = std::make_unique<Object3d>("suzanne.obj");
	cameraObj_->Update();

	reticle3d_ = std::make_unique<Reticle3D>();
	reticle3d_->SetRailCamera(this);
	reticle3d_->Initialize();

	controlPoints_ = GenerateSpiralControlPoints(3, 5, 10, 100);

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

	// ラインの複数オブジェクト
	line3d_->Update();

	// レールの板ポリ
	for (auto& rail : railObj_) {
		rail->Update();
	}

	reticle3d_->Update();

	// 弾を生成
	CreateBullet();

	// 弾
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		(*it)->Update();
		if (!(*it)->GetIsActive()) {
			it = bullets_.erase(it);
		}
		else {
			++it;
		}
	}
}

void RailCamera::Draw()
{
	if (camera_->GetIsDebug()) {
		cameraObj_->Draw();
	}
	for (auto& rail : railObj_) {
		rail->Draw();
	}

	reticle3d_->Draw();

	for (auto& bullet : bullets_) {
		bullet->Draw();
	}

}

void RailCamera::DrawLine()
{
	// ラインの複数描画
	line3d_->Draws();

	for (auto& bullet : bullets_) {
		bullet->LineDraw();
	}
}

void RailCamera::Debug_ImGui()
{
	/*ImGui::Begin("RailCamera");
	ImGui::End();*/
}

void RailCamera::RailCameraMove()
{
	// 時間の進行量
	const float dt = 0.0003f;
	if (!controlPoints_.empty()) {
		// スプライン上の現在の位置を計算
		Vector3 position = CatmullRomPosition(controlPoints_, t_);
		// 少し先の位置を計算し、注視点を決定
		float nextT = t_ + dt;
		Vector3 lookAtPosition = CatmullRomPosition(controlPoints_, nextT);
		// カメラの位置を更新
		cameraObj_->SetPosition(position + offset_);
		if (!camera_->GetIsDebug()) {
			camera_->SetTranslate(position + offset_);
		}
		// 時間を進行
		t_ += dt;
		if (t_ > 1.0f) {
			t_ = 0.0f; // ループさせる場合
		}
		Vector3 velocity = Subtract(lookAtPosition - (offset_ * (dt * 5.0f)), position);

		Vector3 rotate{};
		rotate.y = std::atan2(velocity.x, velocity.z);
		Matrix4x4 rotateMatrixY = MakeRotateYMatrix(-rotate.y);
		Vector3 velocityZ = Transform_(velocity, rotateMatrixY);
		rotate.x = std::atan2(-velocityZ.y, velocityZ.z);
		cameraObj_->SetRotation(rotate);
		if (!camera_->GetIsDebug()) {
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
			if (i % (int)(lineNum / 50) == 0) {
				std::unique_ptr<Object3d> rail = std::make_unique<Object3d>("rail.obj");
				rail->SetRotation(rotate);
				rail->SetPosition(position);
				railObj_.push_back(std::move(rail));
			}
		}
	}
}

void RailCamera::CreateBullet()
{
	if (input_->TriggerKey(DIK_SPACE)) {

		Vector3 velocity = Subtract(reticle3d_->GetWorldPosition(), cameraObj_->GetWorldPosition());
		// 弾を生成
		std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
		bullet->SetRailCamera(this);
		bullet->Initialize(Subtract(cameraObj_->GetPosition(), Vector3{ 0,0.2f,0 }), velocity * 0.05f);
		bullets_.push_back(std::move(bullet));
	}
}

std::vector<Vector3> RailCamera::GenerateSpiralControlPoints(int numTurns, float radius, float height, int pointsPerTurn)
{
	std::vector<Vector3> controlPoints;

	// らせんの生成
	for (int i = 0; i < numTurns * pointsPerTurn; ++i) {
		float angle = (2.0f * pi * i) / pointsPerTurn;  // 円周上の角度
		float currentHeight = (height * i) / (numTurns * pointsPerTurn);  // Y軸方向に高さを増加

		// コントロールポイントの座標を計算
		Vector3 point = {
			radius * std::cos(angle),  // X座標
			currentHeight,             // Y座標（高さ）
			radius * std::sin(angle)   // Z座標
		};

		controlPoints.push_back(point);
	}

	return controlPoints;
}
