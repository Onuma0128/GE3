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
	for (int i = 0; i < global_->GetValue<int>("EnemyManager", "enemyCount"); ++i) {
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		enemy->SetPlayer(player_);
		enemy->Init();
		std::string key = "position" + std::to_string(count);
		enemy->GetTransform()->translation_ = (global_->GetValue<Vector3>("EnemyManager", key));
		enemys_.push_back(std::move(enemy));
		++count;
	}
}

void EnemyManager::GlobalInit()
{
	global_->AddValue<int>("EnemyManager", "enemyCount", 0);
	global_->AddValue<bool>("EnemyManager", "isMove", false);
	global_->AddValue<float>("EnemyManager", "enemyCollisionRadius", 1.0f);


	global_->AddValue<float>("Enemy", "moveSpeed", 0.01f);
	global_->AddValue<float>("Enemy", "slerpSpeed", 0.1f);
}

void EnemyManager::Update()
{
	// 敵の更新
	if (global_->GetValue<bool>("EnemyManager", "isMove")) {
		for (auto& enemy : enemys_) {
			CollisionEnemyToEnemy(enemy.get());
			enemy->Update();
		}
	}
	else {
		int count = 1;
		for (auto& enemy : enemys_) {
			std::string key = "position" + std::to_string(count);
			enemy->GetTransform()->translation_ = (global_->GetValue<Vector3>("EnemyManager", key));
			enemy->Debug_Update();
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
	int newEnemy = global_->GetValue<int>("EnemyManager", "enemyCount");
	if (ImGui::Button("NewEnemy")) {
		newEnemy += 1;
		global_->SetValue<int>("EnemyManager", "enemyCount", newEnemy);
		AddEnemy(newEnemy);
	}
	if (ImGui::Button("deleteEnemy")) {
		std::string key = "position" + std::to_string(newEnemy);
		global_->RemoveKey("EnemyManager", key);
		newEnemy -= 1;
		global_->SetValue<int>("EnemyManager", "enemyCount", newEnemy);
		enemys_.pop_back();
	}

	ImGui::End();
#endif // _DEBUG
}

void EnemyManager::CollisionEnemyToEnemy(Enemy* enemy)
{
	for (auto& enemy1 : enemys_) {
		Vector3 translation1 = enemy1->GetTransform()->translation_;
		if ((translation1 - enemy->GetTransform()->translation_).Length() < global_->GetValue<float>("EnemyManager", "enemyCollisionRadius") && 
			translation1.Length() != enemy->GetTransform()->translation_.Length()) {
			enemy->CollisionEnemy(translation1);
			enemy1->CollisionEnemy(enemy->GetTransform()->translation_);
		}
	}
}

void EnemyManager::AddEnemy(int count)
{
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->SetPlayer(player_);
	enemy->Init();
	enemys_.push_back(std::move(enemy));

	std::string key = "position" + std::to_string(count);
	global_->AddValue<Vector3>("EnemyManager", key, Vector3{});
}
