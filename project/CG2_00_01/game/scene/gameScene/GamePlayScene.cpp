#include "GamePlayScene.h"

#include "CameraManager.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "Input.h"

#include "Object3dBase.h"
#include "SpriteBase.h"
#include "PrimitiveDrawer.h"

#include "titleScene/TitleScene.h"

void GamePlayScene::Initialize()
{
	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	CameraManager::GetInstance()->SetCamera(camera_.get());

	transform_ = std::make_unique<WorldTransform>();

	ModelManager::GetInstance()->LoadModel("resources", "terrain.obj");
	model_ = std::make_unique<Object3d>();
	model_->Initialize("terrain.obj", transform_.get());

	teapotTrans_ = std::make_unique<WorldTransform>();

	ModelManager::GetInstance()->LoadModel("resources", "teapot.obj");
	teapot_ = std::make_unique<Object3d>();
	teapot_->Initialize("teapot.obj", teapotTrans_.get());
	teapotTrans_->parent_ = transform_.get();

}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}

	transform_->rotation_.y += 0.01f;

	teapot_->Update();
	model_->Update();
}

void GamePlayScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();
	
	model_->Draw();
	teapot_->Draw();

	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();
	



	// Lineの描画準備
	PrimitiveDrawer::GetInstance()->DrawBase();




	// Particleの描画

}
