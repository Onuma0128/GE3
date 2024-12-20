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

	std::unique_ptr<Sprite> sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("uvChecker.png");
	sprite_->SetPosition({ 64,64 });
	sprite_->SetSize({ 128,128 });
	sprite_->SetAnchorPoint({ 0.5f,0.5f });
	sprites_.push_back(std::move(sprite_));

	std::unique_ptr<Sprite> sprite1_ = std::make_unique<Sprite>();
	sprite1_->Initialize("Apple.png");
	sprite1_->SetPosition({ 640,64 });
	sprite1_->SetSize({ 128,128 });
	sprite1_->SetAnchorPoint({ 0.5f,0.5f });
	sprites_.push_back(std::move(sprite1_));

	ModelManager::GetInstance()->LoadModel("resources", "suzanne.obj");
	std::unique_ptr<Object3d> object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize("suzanne.obj");
	object3d_->SetRotation({ 0.0f,3.14f,0.0f });
	obj_.push_back(std::move(object3d_));

	emitter_ = std::make_unique<ParticleEmitter>("test");
	ParticleManager::GetInstance()->CreateParticleGroup("test", "uvChecker.png", emitter_.get());
}

void TitleScene::Finalize()
{
	ParticleManager::GetInstance()->Clear();
}

void TitleScene::Update()
{
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

	if (Input::GetInstance()->PushKey(DIK_F)) {
		CameraManager::GetInstance()->SetActiveCamera(0);
	}
	if (Input::GetInstance()->PushKey(DIK_G)) {
		CameraManager::GetInstance()->SetActiveCamera(1);
	}


	for (auto& obj : obj_) {
		obj->Update();
	}
	for (auto& sprite : sprites_) {
		sprite->Update();
	}

	ParticleManager::GetInstance()->Update();
}

void TitleScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();
	for (auto& obj : obj_) {
		obj->Draw();
	}

	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();
	for (auto& sprite : sprites_) {
		sprite->Draw();
	}


	ParticleManager::GetInstance()->Draw();
}
