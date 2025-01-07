#include "AttackStateEnemy.h"

#include "MoveStateEnemy.h"

#include "gameScene/enemyManager/enemy/Enemy.h"
#include "gameScene/player/Player.h"

AttackStateEnemy::AttackStateEnemy(Enemy* enemy) : BaseStateEnemy(enemy) {}

void AttackStateEnemy::Initialize()
{
	offsetQuaternion_ = enemy_->GetTransform()->rotation_;
	attackFrame_ = 0.0f;
}

void AttackStateEnemy::Update()
{
	attackFrame_ += 1.0f / global_->GetValue<float>("Enemy", "attackFrame");

	float pi = static_cast<float>(std::numbers::pi) * 0.5f;
	enemy_->GetTransform()->scale_.x = 1.0f - std::cos(attackFrame_ * pi) * global_->GetValue<float>("Enemy", "attackScale");
	enemy_->GetTransform()->scale_.y = 1.0f + std::cos(attackFrame_ * pi) * global_->GetValue<float>("Enemy", "attackScale");
	enemy_->GetTransform()->scale_.z = 1.0f - std::cos(attackFrame_ * pi) * global_->GetValue<float>("Enemy", "attackScale");
	
	pi = static_cast<float>(std::numbers::pi);
	float angle = std::sin(attackFrame_ * pi);
	Quaternion attackQ = offsetQuaternion_ * Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, angle);
	enemy_->GetTransform()->rotation_ = attackQ;

	if (attackFrame_ >= 1.0f) {
		attackFrame_ = 0.0f;
		enemy_->ChengeState(std::make_unique<MoveStateEnemy>(enemy_));
	}
}

void AttackStateEnemy::Draw()
{
}

void AttackStateEnemy::Finalize()
{
}
