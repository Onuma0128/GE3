#include "EnemyManager.h"

#include "ModelManager.h"

#include "railCamera/RailCamera.h"

void EnemyManager::Initialize()
{
	popEnemy_ = { false,false,false,false,false };
	popEnemyTriggered_ = { false,false,false,false,false };

	for (int i = 0; i < 3; ++i) {
		std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
		sprite->Initialize("number/0.png");
		sprite->SetSize(Vector2{ 64,64 });
		sprite->SetAnchorPoint({ 0.5f,0.5f });
		sprite->SetPosition(Vector2{ 1088 + (float)i * 64,656 });
		sprites_.push_back(std::move(sprite));
	}
}

void EnemyManager::Update()
{
	CreateEnemy();

	for (auto it = enemys_.begin(); it != enemys_.end();) {
		(*it)->Update();
		if (!(*it)->GetIsActive()) {
			it = enemys_.erase(it);
			return;
		}
		if ((*it)->GetState() == Enemy::State::Dead) {
			it = enemys_.erase(it);
			gameScore_ += 50;
		}
		else {
			++it;
		}
	}

	SpriteUpdate();
}

void EnemyManager::SpriteUpdate()
{
	int score = gameScore_;
	for (int i = 3 - 1; i >= 0; --i) {
		int digit = score % 10; // 各桁の数値を取得
		score /= 10; // スコアを1桁下げる

		// 数値に応じた画像を設定
		sprites_[i]->SetTexture("number/" + std::to_string(digit) + ".png");

		sprites_[i]->Update();
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemys_) {
		enemy->Draw();
	}

#ifdef _DEBUG

	ImGui::Begin("EnemyManager");
	if (ImGui::Button("flags reset")) {
		popEnemy_ = { false,false,false,false,false };
		popEnemyTriggered_ = { false,false,false,false,false };
	}
	ImGui::End();

#endif // _DEBUG
}

void EnemyManager::DrawSprite()
{
	for (auto& sprite : sprites_) {
		sprite->Draw();
	}
}

void EnemyManager::CreateEnemy()
{
	if (!popEnemyTriggered_[0] && camera_->GetCameraTime() > 0.0f) {
		popEnemy_[0] = true;
	}
	if (!popEnemyTriggered_[1] && camera_->GetCameraTime() > 0.3f) {
		popEnemy_[1] = true;
	}
	if (!popEnemyTriggered_[2] && camera_->GetCameraTime() > 0.4f) {
		popEnemy_[2] = true;
	}
	if (!popEnemyTriggered_[3] && camera_->GetCameraTime() > 0.6f) {
		popEnemy_[3] = true;
	}
	/*if (!popEnemyTriggered_[4] && camera_->GetCameraTime() > 0.75f) {
		popEnemy_[4] = true;
	}*/

	for (size_t i = 0; i < popEnemy_.size(); ++i) {
		if (popEnemy_[i]) {
			// 各フラグに対応する設定を `GlobalVariables` から取得
			std::string groupName = "EnemyPop" + std::to_string(i);
			int32_t popCount = global->GetValue<int32_t>(groupName, "popCount");

			// popCount の数だけエネミーを生成
			for (int j = 0; j < popCount; ++j) {
				auto enemy = std::make_unique<Enemy>();

				Vector3 cameraPos = camera_->GetCameraObj()->GetPosition();
				Vector3 position = global->GetValue<Vector3>(groupName, "position" + std::to_string(j));
				Vector3 velocity = global->GetValue<Vector3>(groupName, "velocity" + std::to_string(j));

				// エネミーの初期位置と速度を設定して初期化
				enemy->SetFilePath("enemyJet.obj");
				enemy->Initialize(position + cameraPos, velocity);

				// エネミーをリストに追加
				enemys_.push_back(std::move(enemy));
			}
			popEnemy_[i] = false;
			popEnemyTriggered_[i] = true;
		}
	}
}
