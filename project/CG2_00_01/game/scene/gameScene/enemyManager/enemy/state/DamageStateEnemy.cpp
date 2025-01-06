#include "DamageStateEnemy.h"

#include "MoveStateEnemy.h"

#include "gameScene/enemyManager/enemy/Enemy.h"
#include "gameScene/player/Player.h"

DamageStateEnemy::DamageStateEnemy(Enemy* enemy) :BaseStateEnemy(enemy) {}

void DamageStateEnemy::Initialize()
{
	damageFrame_ = 0.0f;
	velocityY = 1.0f;
}

void DamageStateEnemy::Update()
{
	if (damageFrame_ == 0.0f) {
		int hp = enemy_->GetHP() - 1;
		enemy_->SetHP(hp);
	}

	damageFrame_ += 1.0f / global_->GetValue<float>("Enemy", "damageFrame");

	if (enemy_->GetHP() <= 0) {
		enemy_->GetTransform()->scale_ = Vector3{ 1.0f,1.0f,1.0f } * (1.0f - damageFrame_);
	}

	enemy_->GetTransform()->translation_ += enemy_->GetVelocity() * (1.0f - damageFrame_);

	if (enemy_->GetPlayer()->GetPlayerAnima()->GetCombo3Frame() > 2.0f) {
		velocityY -= 0.1f;
		enemy_->GetTransform()->translation_.y += velocityY;
		if (enemy_->GetTransform()->translation_.y <= 0.5f) {
			enemy_->GetTransform()->translation_.y = 0.5f;
		}
	}
	if (enemy_->GetPlayer()->GetPlayerAnima()->GetCombo3Frame() == 0.0f && enemy_->GetTransform()->translation_.y > 0.5f) {
		velocityY -= 0.1f;
		enemy_->GetTransform()->translation_.y += velocityY;
		if (enemy_->GetTransform()->translation_.y <= 0.5f) {
			enemy_->GetTransform()->translation_.y = 0.5f;
		}
	}

	if (damageFrame_ >= 1.0f) {
		damageFrame_ = 0.0f;
		velocityY = 1.0f;
		enemy_->SetIsDamage(false);
		enemy_->ChengeState(std::make_unique<MoveStateEnemy>(enemy_));
	}
}

void DamageStateEnemy::Draw()
{
}

void DamageStateEnemy::Finalize()
{
}
