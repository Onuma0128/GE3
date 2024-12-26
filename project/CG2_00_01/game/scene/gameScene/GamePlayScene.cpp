#include "GamePlayScene.h"

#include "Input.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"

#include "Object3dBase.h"
#include "SpriteBase.h"
#include "PrimitiveDrawer.h"

#include "imgui.h"
#include "titleScene/TitleScene.h"

void GamePlayScene::Initialize()
{
	camera_ = std::make_unique<GameCamera>();
	camera_->Init();

	ModelManager::GetInstance()->LoadModel("resources", "ground.obj");

	groundTransform_ = std::make_unique<WorldTransform>();
	ground_ = std::make_unique<Object3d>();
	ground_->Initialize("ground.obj", groundTransform_.get());

	player_ = std::make_unique<Player>();
	player_->Init();

	camera_->SetPlayer(player_.get());

	emitter_ = std::make_unique<ParticleEmitter>("field");
	ParticleManager::GetInstance()->CreateParticleGroup("field", "white1x1.png", emitter_.get());
}

void GamePlayScene::Finalize()
{
	ParticleManager::GetInstance()->Clear();
}

void GamePlayScene::Update()
{

	if (Input::GetInstance()->PushKey(DIK_P)) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}

	ground_->Update();

	camera_->Update();

	player_->Update();

	// 全パーティクルの更新
	ParticleManager::GetInstance()->Update();
}

void GamePlayScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();

	ground_->Draw();

	player_->Draw();

	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();



	// Lineの描画準備
	PrimitiveDrawer::GetInstance()->DrawBase();


	// 全パーティクルの描画
	ParticleManager::GetInstance()->Draw();

}
