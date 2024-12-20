#include "TitleScene.h"
#include "Input.h"

#include "Logger.h"
#include "CameraManager.h"
#include "SpriteBase.h"
#include "Object3dBase.h"
#include "ModelManager.h"
#include "ParticleManager.h"
#include "AudioManager.h"

#include "SceneManager.h"

#include "imgui.h"

void TitleScene::Initialize()
{
	// Cameraの初期化
	// ===============
	// Cameraのポインタを初期化
	// CameraManagerに登録
	// ===============
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	CameraManager::GetInstance()->SetCamera(camera_.get());

	camera1_ = std::make_unique<Camera>();
	camera1_->Initialize();
	camera1_->SetRotate(Vector3{ 0.26f,1.57f,0.0f });
	camera1_->SetTranslate(Vector3{ -15.0f,4.0f,0.0f });
	CameraManager::GetInstance()->SetCamera(camera1_.get());

	// プレイヤーの初期化
	player_ = std::make_unique<Player>();
	player_->Init();

	// 入力の初期化
	inputHandler_ = std::make_unique<InputHandler>();
	inputHandler_->AssignMoveLeftCommand2PressKeyA();
	inputHandler_->AssignMoveRightCommand2PressKeyD();
}

void TitleScene::Finalize()
{
	AudioManager::GetInstance()->Finalize();
}

void TitleScene::Update()
{
	if (Input::GetInstance()->PushKey(DIK_F1)) {
		CameraManager::GetInstance()->SetActiveCamera(0);
	}
	if (Input::GetInstance()->PushKey(DIK_F2)) {
		CameraManager::GetInstance()->SetActiveCamera(1);
	}

	command_ = inputHandler_->HandleInput();

	if (command_) {
		command_->Exec(*player_.get());
	}

	player_->Update();

}

void TitleScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();
	



	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();
	
	player_->Draw();


	// Particleの描画
	ParticleManager::GetInstance()->Draw();
}
