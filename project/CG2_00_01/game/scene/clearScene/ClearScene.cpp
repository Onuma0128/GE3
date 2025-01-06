#include "ClearScene.h"

#include "CameraManager.h"
#include "SceneManager.h"
#include "Input.h"

#include "Object3dBase.h"
#include "SpriteBase.h"
#include "PrimitiveDrawer.h"

void ClearScene::Initialize()
{
	// Cameraの初期化
	// ===============
	// Cameraのポインタを初期化
	// CameraManagerに登録
	// ===============
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	CameraManager::GetInstance()->SetCamera(camera_.get());

	audio_ = std::make_unique<Audio>();

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("ui/GameClear.png");

	fade_ = std::make_unique<FadeScene>();
	fade_->Init(0.0f);
}

void ClearScene::Finalize()
{
}

void ClearScene::Update()
{
	if (Input::GetInstance()->TriggerGamepadButton(XINPUT_GAMEPAD_A) && !isFade_) {
		isFade_ = true;
		audio_->SoundPlayWave("Button.wav", 0.5f);
	}
	if (isFade_) {
		fade_->FadeIn("Title", Vector3{ 0.0f,0.0f,0.0f }, 120.0f);
	}

	sprite_->Update();
}

void ClearScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();




	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();

	sprite_->Draw();

	fade_->Draw();

	// Lineの描画準備
	PrimitiveDrawer::GetInstance()->DrawBase();




}
