#include "MyGame.h"

#include "Camera.h"
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
	SceneManager::GetInstance()->ChangeScene("Title");

	GlobalVariables::GetInstance()->LoadFiles();
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

	GlobalVariables::GetInstance()->Update();

	// カメラのImGui
	Camera::GetInstance()->CameraImGui();

	// カメラの更新
	Camera::GetInstance()->Update();

	// ライトの更新
	LightManager::GetInstance()->Update();

	SceneManager::GetInstance()->Draw();

	// 描画後の処理
	directXEngine_->PostDraw();
}
