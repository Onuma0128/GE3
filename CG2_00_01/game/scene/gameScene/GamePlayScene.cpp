#include "GamePlayScene.h"

#include "Camera.h"
#include "Input.h"
#include "titleScene/TitleScene.h"
#include "SceneManager.h"
#include "PrimitiveDrawer.h"

void GamePlayScene::Initialize()
{
	ground_ = std::make_unique<Object3d>("terrain.obj");

	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize();

	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetRailCamera(railCamera_.get());
	enemyManager_->Initialize();

	collisionManager_ = std::make_unique<CollisionManager>();
}

void GamePlayScene::Finalize()
{
	
}

void GamePlayScene::Update()
{
	ground_->Update();

	railCamera_->Update();

	enemyManager_->Update();

	CheckAllCollisions();
}

void GamePlayScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();
	//ground_->Draw();

	railCamera_->Debug_ImGui();
	railCamera_->Draw();

	enemyManager_->Draw();

	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();
	railCamera_->DrawSprite();

	// Line描画準備
	PrimitiveDrawer::GetInstance()->DrawBase();

	railCamera_->DrawLine();
}

void GamePlayScene::CheckAllCollisions()
{
	collisionManager_->Reset();

	for (auto& bullet : railCamera_->GetBullets()) {
		collisionManager_->AddCollider(bullet.get());
	}
	for (auto& enemy : enemyManager_->GetEnemys()) {
		collisionManager_->AddCollider(enemy.get());
	}

	collisionManager_->CheckAllCollisions();
}
