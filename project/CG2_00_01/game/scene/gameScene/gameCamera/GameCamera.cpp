#include "GameCamera.h"

#include "CameraManager.h"
#include "Vector3.h"

#include "gameScene/player/Player.h"

void GameCamera::Init()
{
	GlobalInit();

	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetRotate(Vector3{ 0.33f,0.0f,0.0f });
	camera_->SetTranslate(Vector3{ 0.0f,7.0f,-22.0f });
	CameraManager::GetInstance()->SetCamera(camera_.get());
	camera_->Update();

	offset_ = camera_->GetTranslate();
}

void GameCamera::GlobalInit()
{
	global_->AddValue<float>("CameraShake", "duration", 0.5f);
	global_->AddValue<float>("CameraShake", "intensity", 0.2f);
}

void GameCamera::Update()
{
	if (player_->GetIsShake()) {
		time_ += deltaTime_;
		duration_ = global_->GetValue<float>("CameraShake", "duration");
		intensity_ = global_->GetValue<float>("CameraShake", "intensity");
		Vector3 shake = Vector3{}.Shake(time_, duration_, intensity_);
		camera_->SetTranslate(offset_ + shake);
		if (time_ > duration_) {
			player_->SetIsShake(false);
			time_ = 0.0f;
		}
	}
}
