#include "MyGame.h"
#include "LightManager.h"

void MyGame::Initialize()
{
	Framework::Initialize();

	scene_ = new TitleScene();
	scene_->Initialize();
}

void MyGame::Finalize()
{
	scene_->Finalize();
	delete scene_;

	winApp_->Finalize();

	Framework::Finalize();
}

void MyGame::Update()
{
	Framework::Update();

	scene_->Update();
}

void MyGame::Draw()
{
	// 描画前の処理
	directXEngine_->PreDraw();

	// 描画処理
	directXEngine_->Draw();

	// ライトの更新
	LightManager::GetInstance()->Update();

	scene_->Draw();

	// 描画後の処理
	directXEngine_->PostDraw();
}
