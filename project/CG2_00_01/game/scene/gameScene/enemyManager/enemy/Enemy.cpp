#include "Enemy.h"

#include "gameScene/player/Player.h"

void Enemy::OnCollision(std::string& name)
{
	// 衝突判定は3パターン

	// 1 攻撃を与えた
	if (name == "player") {

	}

	// 2 攻撃を食らった(1,2コンボ目)
	if (name == "sword") {
		if (player_->GetIsAttack() && player_->GetPlayerAnima()->GetCombo3Frame() == 0.0f) {
			transform_->translation_ += (transform_->translation_ - player_->GetTransform()->translation_);
		}
		// 3 攻撃を食らった(3コンボ目)
		else {
			transform_->translation_ += (transform_->translation_ - player_->GetTransform()->translation_) * 3.0f;
		}
	}
}

Vector3 Enemy::GetCenterPosition() const
{
	return Vector3{}.Transform(transform_->matWorld_);
}

std::string Enemy::GetColliderName() const
{
	return "enemy";
}

float Enemy::GetRadius() const
{
	return global_->GetValue<float>("Collider", "enemyRadius");
}

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
