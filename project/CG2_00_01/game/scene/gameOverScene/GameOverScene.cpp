#include "GameOverScene.h"

#include "CameraManager.h"
#include "SceneManager.h"
#include "Input.h"

#include "Object3dBase.h"
#include "SpriteBase.h"
#include "PrimitiveDrawer.h"

void GameOverScene::Initialize()
{
	// Cameraの初期化
	// ===============
	// Cameraのポインタを初期化
	// CameraManagerに登録
	// ===============
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	CameraManager::GetInstance()->SetCamera(camera_.get());
}

void GameOverScene::Finalize()
{
}

void GameOverScene::Update()
{
}

void GameOverScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();




	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();




	// Lineの描画準備
	PrimitiveDrawer::GetInstance()->DrawBase();




}
