#include "MoveState.h"

#include "gameScene/player/Player.h"
#include "gameScene/player/state/JumpState.h"

MoveState::MoveState(Player* player) : BaseState(player) {}

void MoveState::Initialize()
{
	
}

void MoveState::Update()
{
	player_->GetMoveEmitter()->SetIsCreate(false);
	
	if (input_->TriggerKey(DIK_SPACE)) {
		Vector3 velocity = player_->GetVelocity();
		velocity.y = global_->GetValue<float>("Player", "velocityY");
		player_->SetVelocity(velocity);

		Vector3 acceleration = player_->GetAcceleration();
		acceleration.y = global_->GetValue<float>("Player", "accelerationY");
		player_->SetAcceleration(acceleration);

		player_->ChengeState(std::make_unique<JumpState>(player_));
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
		player_->GetMoveEmitter()->SetIsCreate(true);
		velocity.Normalize();
		player_->SetVelocity(velocity);
	}

	// 回転の処理
	float newRotateY = std::atan2(player_->GetVelocity().x, player_->GetVelocity().z);
	float nowRotateY = player_->GetModel()->GetRotation().y;
	float rotate = LerpShortAngle(nowRotateY, newRotateY, 0.2f);
	player_->GetModel()->SetRotation(Vector3{0.0f,rotate ,0.0f});

	// 移動の処理
	Vector3 translate = player_->GetModel()->GetPosition();
	translate += velocity * 0.03f;
	player_->GetModel()->SetPosition(translate);

	// 影の処理
	translate.y = 0.01f;
	player_->GetShadowModel()->SetRotation(player_->GetModel()->GetRotation());
	player_->GetShadowModel()->SetPosition(translate);

	// エミッターの処理
	player_->GetMoveEmitter()->SetPosition(player_->GetModel()->GetPosition());
	Vector3 acceleration = player_->GetVelocity() * -1.0f;
	acceleration.y = 1.0f;
	player_->GetMoveEmitter()->SetAcceleration(acceleration);
}

void MoveState::Draw()
{
}

void MoveState::Finalize()
{
}

float MoveState::LerpShortAngle(float a, float b, float t)
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