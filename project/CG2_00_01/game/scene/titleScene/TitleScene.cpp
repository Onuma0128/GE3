#include "TitleScene.h"

#include "CameraManager.h"
#include "SceneManager.h"
#include "Input.h"

#include "Object3dBase.h"
#include "SpriteBase.h"
#include "PrimitiveDrawer.h"
#include "TrailEffectBase.h"

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

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("ui/Title.png");

	audio_ = std::make_unique<Audio>();

	fade_ = std::make_unique<FadeScene>();
	fade_->Init(0.0f);
}

void TitleScene::Finalize()
{
	
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerGamepadButton(XINPUT_GAMEPAD_A) && !isFade_) {
		isFade_ = true;
		audio_->SoundPlayWave("Button.wav", 0.5f);
	}
	if (isFade_) {
		fade_->FadeIn("Game", Vector3{ 0.0f,0.0f,0.0f }, 120.0f);
	}

	sprite_->Update();
}

void TitleScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();
	



	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();
	
	sprite_->Draw();

	fade_->Draw();

	// Lineの描画準備
	PrimitiveDrawer::GetInstance()->DrawBase();




	TrailEffectBase::GetInstance()->DrawBase();

}
