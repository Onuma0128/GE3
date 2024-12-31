#include "EnemyManager.h"

#include <string>

#include "imgui.h"

#include "gameScene/player/Player.h"

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init()
{
	GlobalInit();

	int count = 1;
	for (int i = 0; i < global_->GetValue<int>("EnemyManager", "EnemyCount"); ++i) {
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		enemy->SetPlayer(player_);
		enemy->Init();
		std::string key = "position" + std::to_string(count);
		enemy->SetPosition(global_->GetValue<Vector3>("EnemyManager", key));
		enemys_.push_back(std::move(enemy));
		++count;
	}
}

void EnemyManager::GlobalInit()
{
	global_->AddValue<int>("EnemyManager", "EnemyCount", 0);
	global_->AddValue<bool>("EnemyManager", "isMove", false);
}

void EnemyManager::Update()
{
	// 敵の更新
	if (global_->GetValue<bool>("EnemyManager", "isMove")) {
		for (auto& enemy : enemys_) {
			enemy->Update();
		}
	}
	else {
		int count = 1;
		for (auto& enemy : enemys_) {
			std::string key = "position" + std::to_string(count);
			enemy->SetPosition(global_->GetValue<Vector3>("EnemyManager", key));
			enemy->Update();
			++count;
		}
	}
}

void EnemyManager::Draw()
{
	// 敵の描画
	for (auto& enemy : enemys_) {
		enemy->Draw();
	}
}

void EnemyManager::Debug_ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("AddEnemy");
	// 敵を追加
	int newEnemy = global_->GetValue<int>("EnemyManager", "EnemyCount");
	if (ImGui::Button("NewEnemy")) {
		newEnemy += 1;
		global_->SetValue<int>("EnemyManager", "EnemyCount", newEnemy);
		AddEnemy(newEnemy);
	}
	if (ImGui::Button("deleteEnemy")) {
		std::string key = "position" + std::to_string(newEnemy);
		global_->RemoveKey("EnemyManager", key);
		newEnemy -= 1;
		global_->SetValue<int>("EnemyManager", "EnemyCount", newEnemy);
		enemys_.pop_back();
	}

	ImGui::End();
#endif // _DEBUG
}

void EnemyManager::AddEnemy(int count)
{
	std::unique_ptr<Enemy> enemy_ = std::make_unique<Enemy>();
	enemy_->SetPlayer(player_);
	enemy_->Init();
	enemys_.push_back(std::move(enemy_));

	std::string key = "position" + std::to_string(count);
	global_->AddValue<Vector3>("EnemyManager", key, Vector3{});
}
