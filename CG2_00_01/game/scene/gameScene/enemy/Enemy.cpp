#include "Enemy.h"

Vector3 Enemy::GetCenterPosition() const
{
	return enemy_->GetWorldPosition();
}

std::string Enemy::GetColliderName() const
{
	return "enemy";
}

float Enemy::GetRadius() const
{
	return 1.0f;
}

void Enemy::OnCollision()
{
	state_ = Enemy::State::Dead;
}

void Enemy::Initialize(const Vector3& position, const Vector3& velocity)
{
	enemy_ = std::make_unique<Object3d>(filePath_);
	enemy_->SetPosition(position);
	velocity_ = velocity;
	state_ = Enemy::State::Move;
}

void Enemy::Update()
{
	switch (state_)
	{
	case Enemy::State::Move:
		Vector3 translate = enemy_->GetPosition();
		translate = translate + velocity_;
		enemy_->SetPosition(translate);
		break;
	case Enemy::State::Dead:
		break;
	default:
		break;
	}
	enemy_->Update();
}

void Enemy::Draw()
{
	enemy_->Draw();
}

void Enemy::SetFilePath(const std::string& filePath)
{
	filePath_ = filePath;
}
