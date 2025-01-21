#include "GameCamera.h"

#include <numbers>

#include "CameraManager.h"
#include "Vector3.h"

#include "gameScene/player/Player.h"

void GameCamera::Init()
{
	GlobalInit();

	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetRotate(global_->GetValue<Vector3>("CameraOffset", "rotation"));
	Matrix4x4 rotateMatrix = Matrix4x4::Rotate(camera_->GetRotate());
	Vector3 translation = global_->GetValue<Vector3>("CameraOffset", "translation").Transform(rotateMatrix);
	camera_->SetTranslate(translation + global_->GetValue<Vector3>("Player", "position"));
	CameraManager::GetInstance()->SetCamera(camera_.get());
	CameraManager::GetInstance()->SetActiveCamera(0);
	camera_->Update();

	playerDamageShake_.time_ = 0.0f;
	playerDamageShake_.duration_ = 0.5f; 
	playerDamageShake_.intensity_ = 0.2f;
	playerDamageShake_.deltaTime_ = 1.0f / 60.0f;

	hitStopShake_.time_ = 0.0f;
	hitStopShake_.duration_ = 0.5f;
	hitStopShake_.intensity_ = 0.2f;
	hitStopShake_.deltaTime_ = 1.0f / 10.0f;
}

void GameCamera::GlobalInit()
{
	global_->AddValue<Vector3>("CameraOffset", "rotation", Vector3{});
	global_->AddValue<Vector3>("CameraOffset", "translation", Vector3{});

	global_->AddValue<float>("CameraShake", "duration", 0.5f);
	global_->AddValue<float>("CameraShake", "intensity", 0.2f);

	global_->AddValue<float>("CameraShake", "hitStopBuration", 0.5f);
	global_->AddValue<float>("CameraShake", "hitStopIntensity", 0.2f);
}

void GameCamera::Update()
{
	const float rotationY = Input::GetInstance()->GetGamepadRightStickX() * 0.03f;
	Vector3 rotation = camera_->GetRotate();
	destinationAngleY += rotationY;
	rotation.y = LerpShortAngle(rotation.y, destinationAngleY, 0.1f);
	camera_->SetRotate(rotation);

	Matrix4x4 rotateMatrix = Matrix4x4::Rotate(rotation);
	Vector3 translation = global_->GetValue<Vector3>("CameraOffset", "translation").Transform(rotateMatrix);

	camera_->SetTranslate(player_->GetTransform()->translation_ + translation);
	offset_ = camera_->GetTranslate();

	if (player_->GetIsShake()) {
		playerDamageShake_.time_ += playerDamageShake_.deltaTime_;
		playerDamageShake_.duration_ = global_->GetValue<float>("CameraShake", "duration");
		playerDamageShake_.intensity_ = global_->GetValue<float>("CameraShake", "intensity");
		Vector3 shake = Vector3{}.Shake(playerDamageShake_.time_, playerDamageShake_.duration_, playerDamageShake_.intensity_);
		camera_->SetTranslate(offset_ + shake);
		if (playerDamageShake_.time_ > playerDamageShake_.duration_) {
			player_->SetIsShake(false);
			playerDamageShake_.time_ = 0.0f;
		}
	}
	if (isShake_) {
		hitStopShake_.time_ += hitStopShake_.deltaTime_;
		hitStopShake_.duration_ = global_->GetValue<float>("CameraShake", "hitStopBuration");
		hitStopShake_.intensity_ = global_->GetValue<float>("CameraShake", "hitStopIntensity");
		Vector3 shake = Vector3{}.Shake(hitStopShake_.time_, hitStopShake_.duration_, hitStopShake_.intensity_);
		camera_->SetTranslate(offset_ + shake);
		if (hitStopShake_.time_ > hitStopShake_.duration_) {
			isShake_ = false;
			hitStopShake_.time_ = 0.0f;
		}
	}
}

float GameCamera::LerpShortAngle(float a, float b, float t)
{
	float diff = b - a;

	// 角度を [-2PI, +2PI] に補正する
	while (diff > 2 * std::numbers::pi_v<float>) {
		diff -= 2 * std::numbers::pi_v<float>;
	}
	while (diff < -2 * std::numbers::pi) {
		diff += 2 * std::numbers::pi_v<float>;
	}

	// 角度を [-PI, +PI] に補正する
	if (diff > std::numbers::pi) {
		diff -= 2 * std::numbers::pi_v<float>;
	}
	else if (diff < -std::numbers::pi_v<float>) {
		diff += 2 * std::numbers::pi_v<float>;
	}

	// 線形補間を行う
	return a + t * diff;
}
