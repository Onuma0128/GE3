#include "Move.h"

#include "titleScene/player/Player.h"

#include "cmath"

Move::Move(Player* player) :PlayerBase(player) {}

void Move::Initialize()
{

}

void Move::Update()
{
	const float kSpeed = 0.05f;
	velocity_ = {};

	if (input_->PushKey(DIK_W)) {
		velocity_.z = 1.0f;
	}
	if (input_->PushKey(DIK_S)) {
		velocity_.z = -1.0f;
	}
	if (input_->PushKey(DIK_A)) {
		velocity_.x = -1.0f;
	}
	if (input_->PushKey(DIK_D)) {
		velocity_.x = 1.0f;
	}
	if (velocity_.x != 0 || velocity_.z != 0) {
		velocity_.Normalize();
		newRotateY = std::atan2(velocity_.x, velocity_.z);
	}
	// 回転を計算
	float angleY = LerpShortAngle(player_->GetPlayerObj()->GetRotation().y, newRotateY, 0.2f);
	player_->GetPlayerObj()->SetRotation(Vector3{ 0.0f,angleY,0.0f });
	// 移動を計算
	Vector3 translate = player_->GetPlayerObj()->GetPosition();
	player_->GetPlayerObj()->SetPosition(translate + (velocity_ * kSpeed));
}

void Move::Draw()
{
}

float Move::LerpShortAngle(float a, float b, float t)
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
