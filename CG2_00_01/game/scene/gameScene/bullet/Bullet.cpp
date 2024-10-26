#include "Bullet.h"

void Bullet::Initialize(const Vector3 position, const Vector3& velocity)
{
	bullet_ = std::make_unique<Object3d>("teapot.obj");
	bullet_->SetScale({ 0.1f,0.1f,0.1f });
	bullet_->SetPosition(position);
	velocity_ = velocity;
	isActive_ = true;
}

void Bullet::Update()
{
	Move();
	bullet_->Update();
}

void Bullet::Draw()
{
	bullet_->Draw();
}

void Bullet::Move()
{
	bulletFrame_ += 1.0f / 300.0f;
	if (bulletFrame_ > 1.0f) {
		isActive_ = false;
	}

	Vector3 translate = bullet_->GetPosition();
	translate = translate + velocity_;
	bullet_->SetPosition(translate);
}
