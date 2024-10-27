#include "Bullet.h"
#include "railCamera/RailCamera.h"

void Bullet::Initialize(const Vector3 position, const Vector3& velocity)
{
	bullet_ = std::make_unique<Object3d>("Box.obj");
	bullet_->SetScale({ 0.1f,0.1f,0.1f });
	bullet_->SetPosition(position);
	velocity_ = velocity;
	isActive_ = true;

	line_ = std::make_unique<Line3d>();
	line_->Initialize(position, position);
}

void Bullet::Update()
{
	Move();
	bullet_->Update();

	line_->SetPosition(
		camera_->GetCameraObj()->GetWorldPosition() - Vector3{ 0,0.01f,0 },
		bullet_->GetWorldPosition()
	);
	line_->Update();
}

void Bullet::Draw()
{
	bullet_->Draw();
}

void Bullet::LineDraw()
{
	line_->Draw();
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
