#include "JumpState.h"

#include "gameScene/player/Player.h"

#include "gameScene/player/state/AttackState.h"
#include "gameScene/player/state/MoveState.h"

JumpState::JumpState(Player* player) : BaseState(player) {}

void JumpState::Initialize()
{
}

void JumpState::Update()
{
	/* ==================== ジャンプの処理 ==================== */

	Vector3 velocity = player_->GetVelocity();
	Vector3 acceleration = player_->GetAcceleration();

	velocity.y -= acceleration.y;
	player_->GetTransform()->translation_.y += velocity.y;

	player_->SetVelocity(velocity);
	player_->SetAcceleration(acceleration);

	// エミッターの処理
	player_->GetMoveEmitter()->SetPosition(player_->GetTransform()->translation_);

	// 影のサイズ、透明度の処理
	float scale = (50.0f - player_->GetTransform()->translation_.y) / (50.0f - 0.5f);
	player_->GetShadowTransform()->scale_ = Vector3{ scale,0.01f,scale };
	player_->GetShadowModel()->SetColor(Vector4{ 0.0f,0.0f,0.0f,scale });

	/* ==================== ジャンプから攻撃の処理へ ==================== */

	if (player_->GetTransform()->translation_.y > 2.5f) {
		if (input_->TriggerKey(DIK_SPACE)) {
			velocity.y = global_->GetValue<float>("Player", "attackVelocityY") * -1.0f;
			player_->SetVelocity(velocity);
			player_->ChengeState(std::make_unique<AttackState>(player_));
			return;
		}
	}

	/* ==================== ジャンプだったら移動に戻る ==================== */

	if (player_->GetTransform()->translation_.y < 0.5f) {
		player_->GetTransform()->translation_.y = 0.5f;
		velocity.y = 0.0f;
		acceleration.y = 0.0f;
		player_->ChengeState(std::make_unique<MoveState>(player_));
		return;
	}
}

void JumpState::Draw()
{
}

void JumpState::Finalize()
{
}
