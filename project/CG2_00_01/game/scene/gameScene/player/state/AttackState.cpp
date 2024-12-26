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
		attackAnimaFrame_ += 1.0f / global_->GetValue<float>("PlayerAttack", "attackAnimaFrame");
		if (attackAnimaFrame_ >= 1.0f) {
			attackAnimaFrame_ = 1.0f;
		}
		Vector3 velocity = player_->GetVelocity();

		// 回転用のベクトル
		Vector3 targetDirection = { -velocity.x, 0.0f, velocity.y };
		Vector3 currentDirection = Vector3::ExprUnitZ;

		// ベクトルから回転行列を計算
		currentDirection = Vector3::ExprUnitX;
		float angle = static_cast<float>(std::numbers::pi) * 2.0f / global_->GetValue<float>("PlayerAttack", "attackAnimaFrame");
		Quaternion xRotation = Quaternion::MakeRotateAxisAngleQuaternion(currentDirection, angle);
		player_->GetTransform()->rotation_.AddRotation(xRotation);
	}

	/* ==================== 移動の処理に遷移 ==================== */

	else {
		attackAnimaFrame_ = 1.0f;

		player_->GetTransform()->translation_.y += player_->GetVelocity().y;

		// 影のサイズ、透明度の更新処理
		player_->ShadowUpdate();

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
