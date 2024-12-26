#include "MoveState.h"

#include "gameScene/player/Player.h"
#include "gameScene/player/state/JumpState.h"

MoveState::MoveState(Player* player) : BaseState(player) {}

void MoveState::Initialize()
{
	// 回転用のベクトル
	Vector3 targetDirection = { -player_->GetVelocity().x, 0.0f, player_->GetVelocity().z};
	Vector3 currentDirection = Vector3::ExprUnitZ;
	// ベクトルから回転行列を計算
	if (player_->GetVelocity().x != 0.0f || player_->GetVelocity().z != 0.0f) {
		Matrix4x4 rotationMatrix = Matrix4x4::DirectionToDirection(currentDirection, targetDirection);
		yRotation_ = Quaternion::FormRotationMatrix(rotationMatrix);
	}
}

void MoveState::Update()
{
	player_->GetMoveEmitter()->SetIsCreate(false);
	
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerGamepadButton(XINPUT_GAMEPAD_A)) {
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
	velocity.x = input_->GetGamepadLeftStickX();
	velocity.z = input_->GetGamepadLeftStickY();
	// 回転用のベクトル
	Vector3 targetDirection = { -velocity.x, 0.0f, velocity.z };
	Vector3 currentDirection = Vector3::ExprUnitZ;

	if (velocity.x != 0.0f || velocity.z != 0.0f) {
		player_->GetMoveEmitter()->SetIsCreate(true);
		velocity.Normalize();
		player_->SetVelocity(velocity);

		// ベクトルから回転行列を計算
		Matrix4x4 rotationMatrix = Matrix4x4::DirectionToDirection(currentDirection, targetDirection);
		yRotation_ = Quaternion::FormRotationMatrix(rotationMatrix);
	}

	// 回転の処理
	player_->GetTransform()->rotation_.Slerp(yRotation_, 0.1f);

	// 移動の処理
	player_->GetTransform()->translation_ += velocity * global_->GetValue<float>("Player", "moveSpeed");

	// 影の処理
	player_->GetShadowTransform()->translation_.y = 0.01f;
	player_->GetShadowTransform()->rotation_ = player_->GetTransform()->rotation_;
	player_->GetShadowTransform()->translation_ = player_->GetTransform()->translation_;

	// エミッターの処理
	player_->GetMoveEmitter()->SetPosition(player_->GetTransform()->translation_);
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
