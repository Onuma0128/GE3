#include "DamageStateEnemy.h"

#include "MoveStateEnemy.h"
#include "TrailEffect.h"

#include "gameScene/gameTimer/GameTimer.h"
#include "gameScene/enemyManager/enemy/Enemy.h"
#include "gameScene/player/Player.h"
#include "gameScene/enemyManager/enemy/effect/EnemyEffect.h"

DamageStateEnemy::DamageStateEnemy(Enemy* enemy) :BaseStateEnemy(enemy) {}

void DamageStateEnemy::Initialize()
{
	isDamage_ = false;
	damageFrame_ = 0.0f;
	velocityY = 1.0f;

	int count = 0;
	for (auto& effect : enemy_->GetEnemyEffect()->GetHitEffects()) {
		if (effect.alpha_ == 0.0f) {
			Vector3 randomPosition;
			do {
				randomPosition = {
					static_cast<float>(rand() % 11 - 5) * 0.1f,
					static_cast<float>(rand() % 11 - 5) * 0.1f,
					static_cast<float>(rand() % 11 - 5) * 0.1f,
				};
			} while (randomPosition.Length() == 0.0f);

			Transform transform = effect.effect_->GetTransform();
			// サイズ
			int randomScale = rand() % 2;
			if (randomScale == 0) {
				transform.scale = global_->GetValue<Vector3>("EnemyHitEffect", "scale");
			}
			else {
				float scale = global_->GetValue<Vector3>("EnemyHitEffect", "scale").x;
				transform.scale = { scale,scale,scale };
			}
			// 座標
			transform.translate = randomPosition + enemy_->GetTransform()->translation_;
			// 回転
			Vector3 velocity = (transform.translate - enemy_->GetTransform()->GetWorldPosition()).Normalize();
			transform.rotate.y = std::atan2(velocity.x, velocity.z);
			Matrix4x4 rotateMatrixY = Matrix4x4::RotateY(-transform.rotate.y);
			Vector3 velocityZ = Vector3{ velocity }.Transform(rotateMatrixY);
			transform.rotate.x = std::atan2(-velocityZ.y, velocityZ.z);
			effect.effect_->SetTransform(transform);
			// 速度を初期化
			effect.velocity_ = velocity * global_->GetValue<float>("EnemyHitEffect", "velocityPow");
			effect.alpha_ = 1.0f;
			++count;
			if (count == global_->GetValue<int>("EnemyHitEffect", "count")) {
				break;
			}
		}
	}
}

void DamageStateEnemy::Update()
{
	if (damageFrame_ == 0.0f && !isDamage_) {
		int hp = enemy_->GetHP() - 1;
		enemy_->SetHP(hp);
		isDamage_ = true;
	}

	damageFrame_ += 1.0f / global_->GetValue<float>("Enemy", "damageFrame") * GameTimer::GetInstance()->GetDeltaTime();

	if (enemy_->GetHP() <= 0) {
		enemy_->GetTransform()->scale_ = Vector3{ 1.0f,1.0f,1.0f } * (1.0f - damageFrame_);
	}
	else {
		float pi = static_cast<float>(std::numbers::pi) * 1.5f;
		enemy_->GetTransform()->scale_.x = 1.0f + -std::cos(damageFrame_ * pi) * global_->GetValue<float>("Enemy", "damageScale");
		enemy_->GetTransform()->scale_.y = 1.0f + std::cos(damageFrame_ * pi) * global_->GetValue<float>("Enemy", "damageScale");
		enemy_->GetTransform()->scale_.z = 1.0f + -std::cos(damageFrame_ * pi) * global_->GetValue<float>("Enemy", "damageScale");
	}

	enemy_->GetTransform()->translation_ += enemy_->GetVelocity() * global_->GetValue<float>("Enemy", "knockbackPow") * (1.0f - damageFrame_);

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
