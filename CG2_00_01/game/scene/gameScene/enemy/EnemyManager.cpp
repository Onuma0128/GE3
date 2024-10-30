#include "EnemyManager.h"

void EnemyManager::Initialize()
{
	for (int i = 0; i < 5; ++i) {
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>("teapot.obj");
		enemy->Initialize(Vector3{(float)i - 2,0,0}, Vector3{});
		enemys_.push_back(enemy);
	}
}

void EnemyManager::Update()
{
	for (auto& enemy : enemys_) {
		enemy->Update();
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemys_) {
		enemy->Draw();
	}
}
