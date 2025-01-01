#include "MoveStateEnemy.h"

#include "gameScene/enemyManager/enemy/Enemy.h"
#include "gameScene/player/Player.h"

MoveStateEnemy::MoveStateEnemy(Enemy* enemy) : BaseStateEnemy(enemy) {}

void MoveStateEnemy::Initialize()
{
}

void MoveStateEnemy::Update()
{
	// 移動の処理
	const float kSpeed = global_->GetValue<float>("Enemy", "moveSpeed");
	Vector3 playerPos = enemy_->GetPlayer()->GetTransform()->translation_ + (Vector3::ExprUnitY * 0.5f);
	Vector3 velocity = playerPos - enemy_->GetTransform()->translation_;
	if (velocity.x != 0.0f || velocity.z != 0.0f) {
		velocity.Normalize();
	}
	enemy_->GetTransform()->translation_ += velocity * kSpeed;

	if (velocity.x != 0.0f || velocity.z != 0.0f) {
		// 回転ベクトル
		Vector3 targetDirection = { -velocity.x, 0.0f, velocity.z };
		// ベクトルから回転行列を計算
		Matrix4x4 rotationMatrix = Matrix4x4::DirectionToDirection(Vector3::ExprUnitZ, targetDirection);
		Quaternion yRotation = Quaternion::FormRotationMatrix(rotationMatrix);
		// 回転の処理
		enemy_->GetTransform()->rotation_.Slerp(yRotation, global_->GetValue<float>("Enemy", "slerpSpeed"));
	}
}

void MoveStateEnemy::Draw()
{
}

void MoveStateEnemy::Finalize()
{
}
