#include "RailCamera.h"

#include "imgui.h"
#include "cmath"

#include "globalVariables/GlobalVariables.h"

void RailCamera::Initialize()
{
	cameraObj_ = std::make_unique<Object3d>("suzanne.obj");
	cameraObj_->Update();

	reticle3d_ = std::make_unique<Reticle3D>();
	reticle3d_->SetRailCamera(this);
	reticle3d_->Initialize();

	// 保存済みのコントロールポイントをロード
	LoadControlPoints();

	controlObject_ = std::make_unique<Object3d>("Box.obj");
	controlObject_->SetScale(Vector3{ 0.1f,0.1f,0.1f });
	controlObject_->SetPosition(controlPoints_[controlPoints_.size() - 1]);

	// ラインの複数描画
	CreateRail();
	line3d_ = std::make_unique<Line3d>();
	line3d_->Initialize(lines);
	t_ = 0.0f;

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
}

void RailCamera::Update() 
{
	// レールカメラの動き
	if (!fadeMove_) {
		RailCameraMove();
	}

	// カメラのオブジェクト
	cameraObj_->Update();

	// ラインの複数オブジェクト
	line3d_->Update();
	controlObject_->Update();

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

	if (fadeMove_) {
		fade_->Update();
	}
}

void RailCamera::Draw()
{
	if (camera_->GetIsDebug()) {
		cameraObj_->Draw();
	}

	reticle3d_->Draw();
#ifdef _DEBUG
	controlObject_->Draw();
#endif // _DEBUG

	for (auto& bullet : bullets_) {
		bullet->Draw();
	}

}

void RailCamera::DrawSprite()
{
	for (auto& rail : railObj_) {
		rail->Draw();
	}
	if (fadeMove_) {
		fade_->Draw();
	}
}

void RailCamera::DrawLine()
{
#ifdef _DEBUG
	// ラインの複数描画
	line3d_->Draws();

	for (auto& bullet : bullets_) {
		bullet->LineDraw();
	}
#endif // _DEBUG
}

void RailCamera::Debug_ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("RailCamera Control Points");

	// controlPoints_の各ポイントを編集可能にする
	for (size_t i = 0; i < controlPoints_.size(); ++i) {
		Vector3& point = controlPoints_[i];
		std::string label = "Point " + std::to_string(i);

		// Vector3の各成分を調整
		ImGui::DragFloat3(label.c_str(), reinterpret_cast<float*>(&point),0.01f);
	}

	// 次に置くコントロールポジション
	Vector3 translate = controlObject_->GetPosition();
	ImGui::DragFloat3("Box", &translate.x, 0.05f);
	controlObject_->SetPosition(translate);

	// ラインを生成
	if (ImGui::Button("CreateRail")) {
		// コントロールポジションの読み込み
		LoadControlPoints();
		Vector3 lineEndPos = controlPoints_.back();
		Vector3 targetPos = controlObject_->GetPosition();  // 新しい位置
		// 自動補間点の追加
		const int numNewPoints = 19; 
		for (int i = 1; i <= numNewPoints; ++i) {
			float t = static_cast<float>(i) / (numNewPoints + 1);  // 補間の進行度
			Vector3 newControlPos = (1 - t) * lineEndPos + t * targetPos;  // 線形補間
			controlPoints_.push_back(newControlPos);  // 中間点を追加
		}

		// 最終目標位置もコントロールポイントに追加
		controlPoints_.push_back(targetPos);

		t_ = 0.0f;
		CreateRail();
		line3d_->Initialize(lines);
	}
	// 最後に追加したラインを削除
	if (ImGui::Button("DeleteRail")) {
		for (int i = 0; i < 20; ++i) {
			controlPoints_.erase(controlPoints_.end() - 1);
		}
		t_ = 0.0f;
		CreateRail();
		line3d_->Initialize(lines);
	}
	// ラインのセーブ
	if (ImGui::Button("Save Control Points")) {
		SaveControlPoints();
	}
	if (ImGui::Button("t reset")) {
		t_ = 0.9f;
	}

	ImGui::End();

	ImGui::Begin("t");
	ImGui::Text("%f", t_);
	ImGui::End();
#endif // _DEBUG
}

void RailCamera::LoadControlPoints()
{
	auto globalVars = GlobalVariables::GetInstance();

	try {
		controlPoints_.clear();
		int i = 0;
		while (true) {
			std::string key = "ControlPoint_" + std::to_string(i);
			Vector3 point = globalVars->GetValue<Vector3>("RailCamera", key);
			controlPoints_.push_back(point);
			++i;
		}
	}
	catch (const std::runtime_error&) {
		// すべてのコントロールポイントが読み込まれると例外が発生するので終了
	}
}

void RailCamera::SaveControlPoints()
{
	auto globalVars = GlobalVariables::GetInstance();
	globalVars->CreateGroup("RailCamera");

	// controlPoints_ を GlobalVariables に保存
	for (size_t i = 0; i < controlPoints_.size(); ++i) {
		std::string key = "ControlPoint_" + std::to_string(i);
		globalVars->SetValue("RailCamera", key, controlPoints_[i]);
	}

	// ファイルに保存
	globalVars->SaveFile("RailCamera");
}

void RailCamera::RailCameraMove()
{
	// 時間の進行量
	const float rt = 0.05f;
	float dt = 0.0002f;
	if (!controlPoints_.empty()) {
		// スプラインの全体の長さを取得
		float length = 0.0f;
		for (size_t i = 0; i < controlPoints_.size() - 1; ++i) {
			length += Distance(controlPoints_[i], controlPoints_[i + 1]);
		}

		// 正規化された進行量を計算（制御点の数に依存せず一定の速度で進むようにスケール）
		float normalizedDt = dt * (100.0f / length);  // 100は速度の調整用の定数

		// スプライン上の現在の位置を計算
		Vector3 position = CatmullRomPosition(controlPoints_, t_);
		// 少し先の位置を計算し、注視点を決定
		float nextT = t_ + normalizedDt * (rt / dt);
		Vector3 lookAtPosition = CatmullRomPosition(controlPoints_, nextT);

		// カメラの位置を更新
		cameraObj_->SetPosition(position + offset_);
		if (!camera_->GetIsDebug()) {
			camera_->SetTranslate(position + offset_);
		}

		Vector3 velocity = Subtract(lookAtPosition - (offset_ * (dt * 5.0f)), position);

		// 時間を進行
		if (velocity.y < 0) {
			t_ += normalizedDt * 2.0f;
		}
		else {
			t_ += normalizedDt;
		}

		if (t_ >= 0.99f) {
			fadeMove_ = true;
		}

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
	lines.clear();
	railObj_.clear();
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
			if (i % (int)(lineNum / 200) == 0) {
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
		for (int i = 0; i < 2; ++i) {
			// Velocityを計算
			Vector3 cameraPos = { 0.3f + (float)i * -0.6f,-0.2f,0.0f };
			Vector3 velocity = Normalize(Subtract(reticle3d_->GetPosition(), cameraPos));
			// 弾を生成
			std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
			bullet->SetRailCamera(this);
			bullet->Initialize(cameraPos, velocity);
			bullets_.push_back(std::move(bullet));
		}
	}
}

void RailCamera::GenerateSpiralControlPoints(int numTurns, float radius, float height, int pointsPerTurn)
{
	// らせんの開始座標
	Vector3 startOffset = { -29, -2, 11 };

	// らせんの生成
	for (int i = 0; i < numTurns * pointsPerTurn; ++i) {
		float angle = -(2.0f * pi * i) / pointsPerTurn;  // 円周上の角度（最初は0度）
		float currentHeight = (height * i) / (numTurns * pointsPerTurn);  // Y軸方向の高さ（最初は0）

		// コントロールポイントの座標を計算
		Vector3 point = {
			radius * std::cos(angle) + startOffset.x,  // X座標（startOffsetから始まる）
			currentHeight + startOffset.y,             // Y座標（高さ）（startOffsetから始まる）
			radius * std::sin(angle) + startOffset.z   // Z座標（startOffsetから始まる）
		};

		controlPoints_.push_back(point);
	}
}

void RailCamera::GenerateLoopControlPoints(int numPoints, float radius, float centerY, float length)
{
	// 一回転ループの生成
	for (int i = 0; i < numPoints; ++i) {
		// 進行方向を調整するためにX軸に沿って少しずつ進める
		float progress = (static_cast<float>(i) / numPoints) * length;

		// 円周上の角度を計算
		float angle = (2.0f * pi * i) / numPoints;

		// コントロールポイントの座標を計算
		Vector3 point = {
			progress,                  // X軸方向に進む（進行方向）
			centerY + radius * -std::cos(angle),  // 一回転の高さ
			radius * -std::sin(angle)             // Z軸での回転
		};
		Vector3 linePos = { 5,0,-6 };

		controlPoints_.push_back(point + linePos);
	}
}
