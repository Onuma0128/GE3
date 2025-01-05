#include "Enemy.h"

#include "gameScene/player/Player.h"
#include "gameScene/enemyManager/enemy/state/MoveStateEnemy.h"
#include "gameScene/enemyManager/enemy/state/DamageStateEnemy.h"

void Enemy::OnCollision(const std::string& name, const Vector3& position)
{
	// 衝突判定は3パターン

	// 1 攻撃を与えた
	if (name == "player") {

	}

	// 2 攻撃を食らった(1,2コンボ目)
	// 3 攻撃を食らった(3コンボ目)
	if (name == "sword" && player_->GetIsAttack() && !isDamage_) {
		velocity_ = (transform_->translation_ - player_->GetTransform()->translation_) * 0.1f;
		velocity_.y = 0.0f;
		velocity_.Normalize();
		isDamage_ = true;
		ChengeState(std::make_unique<DamageStateEnemy>(this));
		audio_->SoundPlayWave("EnemyDamage.wav", 0.1f);
	}

	// 4 敵同士の当たり判定
	if (name == "enemy") {
		CollisionEnemy(position);
	}
}

Vector3 Enemy::GetCenterPosition() const
{
	return transform_->translation_;
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
	audio_ = std::make_unique<Audio>();

	transform_ = std::make_unique<WorldTransform>();
	float angle = static_cast<float>(std::numbers::pi);
	transform_->rotation_ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, angle);
	model_ = std::make_unique<Object3d>();
	model_->Initialize("box.obj", transform_.get());

	state_ = std::make_unique<MoveStateEnemy>(this);
	state_->Initialize();
}

void Enemy::Update()
{
	state_->Update();

	model_->Update();
}

void Enemy::Draw()
{
	state_->Draw();

	model_->Draw();
}

void Enemy::Debug_Update()
{
	model_->Update();
}

void Enemy::CollisionEnemy(const Vector3& translation)
{
	const float kSpeed = global_->GetValue<float>("Enemy", "moveSpeed") * 5.0f;
	Vector3 velocity = transform_->translation_ - translation;
	velocity.y = 0.0f;
	float distance = velocity.Length();
	if (distance > 0.0f) {
		velocity.Normalize();
		// 衝突距離に応じて速度をスケール
		transform_->translation_ += velocity * kSpeed * (1.0f / distance);
	}
}

void Enemy::ChengeState(std::unique_ptr<BaseStateEnemy> newState)
{
	state_->Finalize();
	state_ = std::move(newState);
	state_->Initialize();
}
