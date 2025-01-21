#include "GamePlayScene.h"

#include "Input.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"

#include "Object3dBase.h"
#include "SpriteBase.h"
#include "PrimitiveDrawer.h"
#include "TrailEffectBase.h"

#include "imgui.h"
#include "titleScene/TitleScene.h"
#include "gameScene/gameTimer/GameTimer.h"

void GamePlayScene::Initialize()
{
	GameTimer::GetInstance()->Init();

	camera_ = std::make_unique<GameCamera>();
	camera_->Init();

	ModelManager::GetInstance()->LoadModel("resources", "ground.obj");

	groundTransform_ = std::make_unique<WorldTransform>();
	ground_ = std::make_unique<Object3d>();
	ground_->Initialize("ground.obj", groundTransform_.get());
	groundTransform_->scale_ = { 10.0f,1.0f,10.0f };
	emitter_ = std::make_unique<ParticleEmitter>("field");
	ParticleManager::GetInstance()->CreateParticleGroup("field", "white1x1.png", emitter_.get());

	skydomeTransform_ = std::make_unique<WorldTransform>();
	skydome_ = std::make_unique<Object3d>();
	skydome_->Initialize("skydome.obj", skydomeTransform_.get());
	skydome_->SetLight(false);
	skydomeTransform_->scale_ = { 100,100,100 };

	player_ = std::make_unique<Player>();
	player_->Init();

	camera_->SetPlayer(player_.get());

	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetPlayer(player_.get());
	enemyManager_->SetGameCamera(camera_.get());
	enemyManager_->Init();


	collisionManager_ = std::make_unique<CollisionManager>();
}

void GamePlayScene::Finalize()
{
	ParticleManager::GetInstance()->Clear();
}

void GamePlayScene::Update()
{
	GameTimer::GetInstance()->Updata();

	ground_->Update();
	skydome_->Update();

	camera_->Update();

	player_->Update();

	enemyManager_->Debug_ImGui();
	enemyManager_->Update();

	// 全ての衝突判定
	CheckAllCollisions();

	// 全パーティクルの更新
	ParticleManager::GetInstance()->Update();
}

void GamePlayScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();

	ground_->Draw();
	skydome_->Draw();

	player_->Draw();

	enemyManager_->Draw();

	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();

	player_->DrawSprite();

	enemyManager_->DrawSprite();

	// Lineの描画準備
	PrimitiveDrawer::GetInstance()->DrawBase();



	// TrailEffectの描画準備
	TrailEffectBase::GetInstance()->DrawBase();

	player_->DrawTrail();

	enemyManager_->DrawEffect();

	// 全パーティクルの描画
	ParticleManager::GetInstance()->Draw();

}

void GamePlayScene::CheckAllCollisions()
{
	collisionManager_->Reset();
	
	collisionManager_->AddCollider(player_.get());

	collisionManager_->AddCollider(player_->GetPlayerAnima()->GetPlayerModels());

	for (auto& enemy : enemyManager_->GetEnemys()) {
		collisionManager_->AddCollider(enemy.get());
	}

	collisionManager_->CheckAllCollisions();
}
