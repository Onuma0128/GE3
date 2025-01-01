#include "DamageState.h"

#include "CameraManager.h"

#include "gameScene/player/Player.h"
#include "gameScene/animation/PlayerAnimation.h"
#include "gameScene/animation/PlayerModels.h"

#include "gameScene/player/state/MoveState.h"
#include "gameScene/player/state/AttackState.h"

DamageState::DamageState(Player* player, PlayerAnimation* playerAnimation) :BaseState(player, playerAnimation) {}

void DamageState::Initialize()
{
	knockbackFrame_ = 0.0f;
}

void DamageState::Update()
{
	// ノックバック開始
	knockbackFrame_ += 1.0f / global_->GetValue<float>("Player", "knockbackFrame");

	// 回転用のベクトル
	Vector3 velocity = player_->GetVelocity();
	Vector3 targetDirection = { velocity.x, 0.0f, -velocity.z };
	Vector3 currentDirection = Vector3::ExprUnitZ;

	if (velocity.x != 0.0f || velocity.z != 0.0f) {
		// ベクトルから回転行列を計算
		Matrix4x4 rotationMatrix = Matrix4x4::DirectionToDirection(currentDirection, targetDirection);
		Quaternion yRotation_ = Quaternion::FormRotationMatrix(rotationMatrix);
		// 回転の処理
		player_->GetTransform()->rotation_.Slerp(yRotation_, global_->GetValue<float>("Player", "slerpSpeed"));
	}

	player_->GetTransform()->translation_ += (velocity * global_->GetValue<float>("Player", "moveSpeed")) * (1.0f - knockbackFrame_);

	if (knockbackFrame_ >= 1.0f) {
		knockbackFrame_ = 0.0f;
		player_->SetVelocity(Vector3{ -velocity.x, 0.0f ,-velocity.z });
		player_->ChengeState(std::make_unique<MoveState>(player_, playerAnimation_));
		return;
	}

	playerAnimation_->Update();
}

void DamageState::Draw()
{
	playerAnimation_->Draw();
}

void DamageState::Finalize()
{
}
