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
	enemy_->SetScale(Vector3{ 0.1f,0.1f ,0.1f });
	velocity_ = velocity;
	state_ = Enemy::State::Move;

	activeFrame_ = 0;
	isActive_ = true;
}

void Enemy::Update()
{
	switch (state_)
	{
	case Enemy::State::Move:
		activeFrame_ += 1.0f / 120.0f;
		if (activeFrame_ > 26.0f) {
			isActive_ = false;
		}
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
	ImGui::Begin("enemy");
	ImGui::Text("%f", activeFrame_);
	ImGui::End();

	enemy_->Draw();
}

void Enemy::SetFilePath(const std::string& filePath)
{
	filePath_ = filePath;
}
