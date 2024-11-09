#include "EnemyManager.h"

#include "railCamera/RailCamera.h"

void EnemyManager::Initialize()
{
	box_ = std::make_unique<Object3d>("Box.obj");
	box_->SetScale({ 0.5f, 0.5f, 0.5f });

	popEnemy_ = { false,false,false,false,false };
	popEnemyTriggered_ = { false,false,false,false,false };

	PopEnemyInitialize();
}

void EnemyManager::PopEnemyInitialize()
{
	/*global->SetValue<int32_t>("EnemyPop0", "popCount", 2);
	global->SetValue<Vector3>("EnemyPop0", "position0", Vector3{ 2,5,-25 });
	global->SetValue<Vector3>("EnemyPop0", "velocity0", Vector3{ 0,0,0.01f });
	global->SetValue<Vector3>("EnemyPop0", "position1", Vector3{ -2,5,-25 });
	global->SetValue<Vector3>("EnemyPop0", "velocity1", Vector3{ 0,0,0.01f });*/

	/*global->SetValue<int32_t>("EnemyPop1", "popCount", 3);
	global->SetValue<Vector3>("EnemyPop1", "position0", Vector3{});
	global->SetValue<Vector3>("EnemyPop1", "velocity0", Vector3{});
	global->SetValue<Vector3>("EnemyPop1", "position1", Vector3{});
	global->SetValue<Vector3>("EnemyPop1", "velocity1", Vector3{});
	global->SetValue<Vector3>("EnemyPop1", "position2", Vector3{});
	global->SetValue<Vector3>("EnemyPop1", "velocity2", Vector3{});*/

	/*global->SetValue<int32_t>("EnemyPop2", "popCount", 3);
	global->SetValue<Vector3>("EnemyPop2", "position0", Vector3{});
	global->SetValue<Vector3>("EnemyPop2", "velocity0", Vector3{});
	global->SetValue<Vector3>("EnemyPop2", "position1", Vector3{});
	global->SetValue<Vector3>("EnemyPop2", "velocity1", Vector3{});
	global->SetValue<Vector3>("EnemyPop2", "position2", Vector3{});
	global->SetValue<Vector3>("EnemyPop2", "velocity2", Vector3{});*/

	/*global->SetValue<int32_t>("EnemyPop3", "popCount", 5);
	global->SetValue<Vector3>("EnemyPop3", "position0", Vector3{ -3.32f,1.64f,-13.06f });
	global->SetValue<Vector3>("EnemyPop3", "velocity0", Vector3{});
	global->SetValue<Vector3>("EnemyPop3", "position1", Vector3{ -11.02f,2.44f,-3.22f });
	global->SetValue<Vector3>("EnemyPop3", "velocity1", Vector3{});
	global->SetValue<Vector3>("EnemyPop3", "position2", Vector3{ -3.32f,5.44f,-13.06f });
	global->SetValue<Vector3>("EnemyPop3", "velocity2", Vector3{});
	global->SetValue<Vector3>("EnemyPop3", "position3", Vector3{ -11.02f,6.24f,-3.22f });
	global->SetValue<Vector3>("EnemyPop3", "velocity3", Vector3{});
	global->SetValue<Vector3>("EnemyPop3", "position4", Vector3{ 3.18f,8.44f,-8.72f });
	global->SetValue<Vector3>("EnemyPop3", "velocity4", Vector3{ 0,0,0.001f });*/

	global->SetValue<int32_t>("EnemyPop4", "popCount", 3);
	global->SetValue<Vector3>("EnemyPop4", "position0", Vector3{});
	global->SetValue<Vector3>("EnemyPop4", "velocity0", Vector3{});
	global->SetValue<Vector3>("EnemyPop4", "position1", Vector3{});
	global->SetValue<Vector3>("EnemyPop4", "velocity1", Vector3{});
	global->SetValue<Vector3>("EnemyPop4", "position2", Vector3{});
	global->SetValue<Vector3>("EnemyPop4", "velocity2", Vector3{});
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
		}
		else {
			++it;
		}
	}

	box_->Update();
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemys_) {
		enemy->Draw();
	}

	box_->Draw();

#ifdef _DEBUG

	ImGui::Begin("EnemyManager");
	if (ImGui::Button("flags reset")) {
		popEnemy_ = { false,false,false,false,false };
		popEnemyTriggered_ = { false,false,false,false,false };
	}

	Vector3 translate = box_->GetPosition();
	ImGui::DragFloat3("position", &translate.x, 0.01f);
	box_->SetPosition(translate);
	ImGui::End();

#endif // _DEBUG
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
