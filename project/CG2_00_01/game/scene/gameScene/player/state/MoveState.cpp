#include "MoveState.h"

#include "CameraManager.h"

#include "gameScene/player/Player.h"
#include "gameScene/animation/PlayerAnimation.h"

#include "gameScene/player/state/AttackState.h"
#include "gameScene/player/state/DashAttackState.h"

MoveState::MoveState(Player* player, PlayerAnimation* playerAnimation) : BaseState(player, playerAnimation) {}

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

	Vector3 velocity = {};
	velocity.x = input_->GetGamepadLeftStickX();
	velocity.z = input_->GetGamepadLeftStickY();
	// 回転用のベクトル
	Vector3 targetDirection = { -velocity.x, 0.0f, velocity.z };
	Vector3 currentDirection = Vector3::ExprUnitZ;
	Matrix4x4 rotateMatrix = Matrix4x4::RotateY(CameraManager::GetInstance()->GetActiveCamera()->GetRotate().y);

	if (velocity.x != 0.0f || velocity.z != 0.0f) {
		player_->GetMoveEmitter()->SetIsCreate(true);
		velocity = velocity.Transform(rotateMatrix);
		velocity.Normalize();
		velocity *= global_->GetValue<float>("Player", "moveSpeed");
		player_->SetVelocity(velocity);

		// ベクトルから回転行列を計算
		Matrix4x4 rotationMatrix = Matrix4x4::DirectionToDirection(currentDirection.Transform(rotateMatrix), targetDirection);
		yRotation_ = Quaternion::FormRotationMatrix(rotationMatrix);
	}

	// 回転の処理
	player_->GetTransform()->rotation_.Slerp(yRotation_, global_->GetValue<float>("Player", "slerpSpeed"));

	// 移動の処理
	player_->GetTransform()->translation_ += velocity;

	// エミッターの処理
	player_->GetMoveEmitter()->SetPosition(player_->GetTransform()->translation_);
	Vector3 acceleration = player_->GetVelocity() * global_->GetValue<float>("Player", "dustAcceleration");
	acceleration.y = global_->GetValue<float>("Player", "dustAccelerationY");
	player_->GetMoveEmitter()->SetAcceleration(acceleration);

	if (input_->TriggerGamepadButton(XINPUT_GAMEPAD_A)) {
		player_->GetMoveEmitter()->SetIsCreate(false);
		player_->ChengeState(std::make_unique<AttackState>(player_, playerAnimation_));
		return;
	}
	if (input_->TriggerGamepadButton(XINPUT_GAMEPAD_B)) {
		player_->GetMoveEmitter()->SetIsCreate(false);
		player_->ChengeState(std::make_unique<DashAttackState>(player_, playerAnimation_));
		return;
	}

	playerAnimation_->NormalAnimation();
	playerAnimation_->Update();
}

void MoveState::Draw()
{
	playerAnimation_->Draw();
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
