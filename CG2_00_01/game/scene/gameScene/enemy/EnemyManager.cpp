#include "EnemyManager.h"

void EnemyManager::Initialize()
{
	for (int i = 0; i < 5; ++i) {
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		enemy->SetFilePath("teapot.obj");
		enemy->Initialize(Vector3{ (float)i - 5.0f,5.0f,5.0f }, Vector3{ 0.01f,0.0f,0.0f });
		enemys_.push_back(std::move(enemy));
	}

}

void EnemyManager::Update()
{
	for (auto it = enemys_.begin(); it != enemys_.end();) {
		(*it)->Update();
		if ((*it)->GetState() == Enemy::State::Dead) {
			it = enemys_.erase(it);
		}
		else {
			++it;
		}
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemys_) {
		enemy->Draw();
	}
}
