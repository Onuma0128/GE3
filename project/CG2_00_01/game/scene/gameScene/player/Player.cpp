#include "Player.h"

#include <numbers>

#include "ModelManager.h"

void Player::Init()
{
	GlobalInit();

	ModelManager::GetInstance()->LoadModel("resources", "box.obj");

	model_ = std::make_unique<Object3d>();
	model_->Initialize("box.obj");
	model_->SetTexture("resources", "uvChecker.png");
	model_->SetPosition(Vector3{ 0.0f,0.5f,0.0f });

	state_ = State::Move;

}

void Player::Update()
{
	switch (state_)
	{
	case Player::State::Move:
		Move();
		break;
	case Player::State::Jump:
		Jump();
		break;
	case Player::State::Attack:
		Attack();
		break;
	default:
		break;
	}

	model_->Update();
}

void Player::Draw()
{
	model_->Draw();
}

void Player::GlobalInit()
{
	global_->AddValue<float>("Player", "velocityY", 1.0f);
	global_->AddValue<float>("Player", "accelerationY", 0.1f);
	global_->AddValue<float>("Player", "attackVelocityY", 0.05f);
}

void Player::Move()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		state_ = State::Jump;
		velocity_.y = global_->GetValue<float>("Player", "velocityY");
		acceleration_.y = global_->GetValue<float>("Player", "accelerationY");
		return;
	}

	Vector3 velocity = {};

	if (input_->PushKey(DIK_W)) {
		velocity.z = 1.0f;
	}
	if (input_->PushKey(DIK_S)) {
		velocity.z = -1.0f;
	}
	if (input_->PushKey(DIK_A)) {
		velocity.x = -1.0f;
	}
	if (input_->PushKey(DIK_D)) {
		velocity.x = 1.0f;
	}
	if (velocity.x != 0.0f || velocity.z != 0.0f) {
		velocity.Normalize();
		velocity_ = velocity;
	}

	// 回転の処理
	float newRotateY = std::atan2(velocity_.x, velocity_.z);
	float nowRotateY = model_->GetRotation().y;
	float rotate = LerpShortAngle(nowRotateY, newRotateY, 0.2f);
	model_->SetRotation(Vector3{ 0.0f,rotate ,0.0f });

	// 移動の処理
	Vector3 translate = model_->GetPosition();
	translate += velocity * 0.03f;
	model_->SetPosition(translate);
}

void Player::Jump()
{
	Vector3 translate = model_->GetPosition();
	velocity_.y -= acceleration_.y;
	translate.y += velocity_.y;
	model_->SetPosition(translate);

	if (model_->GetPosition().y > 2.5f) {
		if (input_->TriggerKey(DIK_SPACE)) {
			state_ = State::Attack;
			velocity_.y = global_->GetValue<float>("Player", "attackVelocityY") * -1.0f;
		}
	}

	if (model_->GetPosition().y < 0.5f) {
		translate.y = 0.5f;
		model_->SetPosition(translate);
		velocity_.y = 0.0f;
		acceleration_.y = 0.0f;
		state_ = State::Move;
	}
}

void Player::Attack()
{
	if (attackAnimaFrame_ < 1.0f) {
		attackAnimaFrame_ += 1.0f / 20.0f;
		if (attackAnimaFrame_ >= 1.0f) {
			attackAnimaFrame_ = 1.0f;
		}
		float rotateX = 2.0f * static_cast<float>(std::numbers::pi) * attackAnimaFrame_;
		float rotateY = std::atan2(velocity_.x, velocity_.z);
		model_->SetRotation(Vector3{ rotateX ,rotateY,0.0f });
	}
	else {
		attackAnimaFrame_ = 1.0f;

		Vector3 translate = model_->GetPosition();
		translate.y += velocity_.y;
		model_->SetPosition(translate);

		if (model_->GetPosition().y < 0.5f) {
			attackAnimaFrame_ = 0.0f;
			translate.y = 0.5f;
			model_->SetPosition(translate);
			velocity_.y = 0.0f;
			acceleration_.y = 0.0f;
			state_ = State::Move;
		}
	}
}

float Player::LerpShortAngle(float a, float b, float t)
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
