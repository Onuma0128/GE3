#include "GamePlayScene.h"

#include "Input.h"
#include "SceneManager.h"
#include "PrimitiveDrawer.h"

void GamePlayScene::Initialize()
{
	ground_ = std::make_unique<Object3d>("terrain.obj");

	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize();
}

void GamePlayScene::Finalize()
{
	
}

void GamePlayScene::Update()
{

	ground_->Update();

	railCamera_->Update();
}

void GamePlayScene::Draw()
{
	railCamera_->Debug_ImGui();

	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();
	ground_->Draw();

	railCamera_->Draw();

	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();


	// Line描画準備
	PrimitiveDrawer::GetInstance()->DrawBase();

	railCamera_->DrawLine();
}
