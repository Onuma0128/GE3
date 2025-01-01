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
	camera_->SetTranslate(translation);
	CameraManager::GetInstance()->SetCamera(camera_.get());
	CameraManager::GetInstance()->SetActiveCamera(0);
	camera_->Update();
}

void GameCamera::GlobalInit()
{
	global_->AddValue<Vector3>("CameraOffset", "rotation", Vector3{});
	global_->AddValue<Vector3>("CameraOffset", "translation", Vector3{});

	global_->AddValue<float>("CameraShake", "duration", 0.5f);
	global_->AddValue<float>("CameraShake", "intensity", 0.2f);
}

void GameCamera::Update()
{
	const float rotationY = Input::GetInstance()->GetGamepadRightStickX() * -0.03f;
	Vector3 rotation = camera_->GetRotate();
	destinationAngleY += rotationY;
	rotation.y = LerpShortAngle(rotation.y, destinationAngleY, 0.1f);
	camera_->SetRotate(rotation);

	Matrix4x4 rotateMatrix = Matrix4x4::Rotate(rotation);
	Vector3 translation = global_->GetValue<Vector3>("CameraOffset", "translation").Transform(rotateMatrix);

	camera_->SetTranslate(player_->GetTransform()->translation_ + translation);

	/*if (player_->GetIsShake()) {
		time_ += deltaTime_;
		duration_ = global_->GetValue<float>("CameraShake", "duration");
		intensity_ = global_->GetValue<float>("CameraShake", "intensity");
		Vector3 shake = Vector3{}.Shake(time_, duration_, intensity_);
		camera_->SetTranslate(offset_ + shake);
		if (time_ > duration_) {
			player_->SetIsShake(false);
			time_ = 0.0f;
		}
	}*/
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
