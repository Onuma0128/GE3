#include "Enemy.h"

#include "gameScene/player/Player.h"

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	transform_ = std::make_unique<WorldTransform>();
	model_ = std::make_unique<Object3d>();
	model_->Initialize("box.obj", transform_.get());
}

void Enemy::Update()
{
	// 移動の処理
	const float kSpeed = global_->GetValue<float>("Enemy", "moveSpeed");
	Vector3 velocity = player_->GetTransform()->translation_ - transform_->translation_;
	if (velocity.x != 0.0f || velocity.z != 0.0f) {
		velocity.Normalize();
	}
	transform_->translation_ += velocity * kSpeed;

	if (velocity.x != 0.0f || velocity.z != 0.0f) {
		// 回転ベクトル
		Vector3 targetDirection = { -velocity.x, 0.0f, velocity.z };
		// ベクトルから回転行列を計算
		Matrix4x4 rotationMatrix = Matrix4x4::DirectionToDirection(Vector3::ExprUnitZ, targetDirection);
		Quaternion yRotation = Quaternion::FormRotationMatrix(rotationMatrix);
		// 回転の処理
		transform_->rotation_.Slerp(yRotation, global_->GetValue<float>("Enemy", "slerpSpeed"));
	}

	model_->Update();
}

void Enemy::Draw()
{
	model_->Draw();
}

void Enemy::Debug_Update()
{
	model_->Update();
}

void Enemy::CollisionEnemy(const Vector3& translation)
{
	const float kSpeed = global_->GetValue<float>("Enemy", "moveSpeed") * 2.0f;
	Vector3 velocity = transform_->translation_ - translation;
	if (velocity.x != 0.0f || velocity.z != 0.0f) {
		velocity.Normalize();
	}
	transform_->translation_ += velocity * kSpeed;
}
