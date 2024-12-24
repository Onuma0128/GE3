#include "AttackState.h"

#include <numbers>

#include "gameScene/player/Player.h"

#include "gameScene/player/state/MoveState.h"

AttackState::AttackState(Player* player) :BaseState(player) {}

void AttackState::Initialize()
{
}

void AttackState::Update()
{
	// エミッターの処理
	player_->GetMoveEmitter()->SetPosition(player_->GetTransform()->translation_);

	/* ==================== 攻撃時のアニメーション ==================== */

	if (attackAnimaFrame_ < 1.0f) {
		attackAnimaFrame_ += 1.0f / 20.0f;
		if (attackAnimaFrame_ >= 1.0f) {
			attackAnimaFrame_ = 1.0f;
		}
		Vector3 velocity = player_->GetVelocity();
		float rotateX = 2.0f * static_cast<float>(std::numbers::pi) * attackAnimaFrame_;
		float rotateY = std::atan2(velocity.x, velocity.z);
		player_->GetTransform()->rotation_ = Vector3{ rotateX ,rotateY,0.0f };
	}

	/* ==================== 移動の処理に遷移 ==================== */

	else {
		attackAnimaFrame_ = 1.0f;

		player_->GetTransform()->translation_.y += player_->GetVelocity().y;

		if (player_->GetTransform()->translation_.y < 0.5f) {
			player_->SetIsShake(true);
			attackAnimaFrame_ = 0.0f;
			player_->GetTransform()->translation_.y = 0.5f;

			Vector3 velocity = player_->GetVelocity();
			Vector3 acceleration = player_->GetAcceleration();
			velocity.y = 0.0f;
			acceleration.y = 0.0f;
			player_->SetVelocity(velocity);
			player_->SetAcceleration(acceleration);
			
			int count = global_->GetValue<int>("PlayerAttackParticle", "count");
			for (int i = 0; i < count; ++i) {
				std::unique_ptr<AttackParticle> particle = std::make_unique<AttackParticle>();
				particle->Init("attackParticle.obj", player_->GetTransform()->translation_);
				player_->SetAttackParticle(std::move(particle));
			}
			player_->ChengeState(std::make_unique<MoveState>(player_));
		}
	}
}

void AttackState::Draw()
{
}

void AttackState::Finalize()
{
}
