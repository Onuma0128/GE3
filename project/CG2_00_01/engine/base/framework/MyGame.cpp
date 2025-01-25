#include "MyGame.h"

#include "CameraManager.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "ParticleManager.h"
#include "SceneFactory.h"
#include "GlobalVariables.h"

void MyGame::Initialize()
{
	Framework::Initialize();

	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	SceneManager::GetInstance()->ChangeScene("Game");

	GlobalVariables::GetInstance()->LoadFiles();
}

void MyGame::Finalize()
{
	winApp_->Finalize();

	Framework::Finalize();
}

void MyGame::Update()
{
	// ImGui受付開始
	imGuiManager_->Begin();

	Framework::Update();

	// シーンの更新
	SceneManager::GetInstance()->Update();

	// GlobalVariablesの更新
	GlobalVariables::GetInstance()->Update();

	// カメラの更新
	CameraManager::GetInstance()->Debug_ImGui();
	CameraManager::GetInstance()->Update();

	// ライトの更新
	LightManager::GetInstance()->Debug_ImGui();
	LightManager::GetInstance()->Update();

	// ImGui受付終了
	imGuiManager_->End();
}

void MyGame::Draw()
{
	// 描画前の処理
	directXEngine_->PreDraw();

	SceneManager::GetInstance()->Draw();

	// ImGuiの描画
	imGuiManager_->Draw();

	// 描画後の処理
	directXEngine_->PostDraw();
}
