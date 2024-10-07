#include "MyGame.h"
#include "LightManager.h"
#include "scene/SceneManager.h"

void MyGame::Initialize()
{
	Framework::Initialize();

	BaseScene* scene_ = new TitleScene();

	SceneManager::GetInstance()->SetNextScene(scene_);
}

void MyGame::Finalize()
{
	winApp_->Finalize();

	Framework::Finalize();
}

void MyGame::Update()
{
	Framework::Update();

	SceneManager::GetInstance()->Update();
}

void MyGame::Draw()
{
	// 描画前の処理
	directXEngine_->PreDraw();

	// 描画処理
	directXEngine_->Draw();

	// ライトの更新
	LightManager::GetInstance()->Update();

	SceneManager::GetInstance()->Draw();

	// 描画後の処理
	directXEngine_->PostDraw();
}
