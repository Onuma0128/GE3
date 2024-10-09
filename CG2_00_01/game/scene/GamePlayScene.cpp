#include "GamePlayScene.h"
#include "Input.h"
#include "SceneManager.h"

void GamePlayScene::Initialize()
{
	ground_ = std::make_unique<Object3d>();
	ground_->Initialize();
	ground_->SetModel("terrain.obj");

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
	

}