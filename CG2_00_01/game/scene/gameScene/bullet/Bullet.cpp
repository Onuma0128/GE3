#include "Bullet.h"
#include "railCamera/RailCamera.h"

Vector3 Bullet::GetCenterPosition() const
{
	return bullet_->GetWorldPosition();
}

std::string Bullet::GetColliderName() const
{
	return "bullet";
}

float Bullet::GetRadius() const
{
	return 0.1f;
}

void Bullet::OnCollision()
{
	isActive_ = false;
}

void Bullet::Initialize(const Vector3 position, const Vector3& velocity)
{
	bullet_ = std::make_unique<Object3d>("Box.obj");
	bullet_->SetScale({ 0.1f,0.1f,0.1f });
	bullet_->SetPosition(position);
	bullet_->SetParent(camera_->GetCameraObj());
	velocity_ = velocity;
	isActive_ = true;

	line_ = std::make_unique<Line3d>();
	line_->Initialize(position, position);
	lineStartPos_ = position;
}

void Bullet::Update()
{
	if (!isActive_) {
		return;
	}

	Move();
	bullet_->Update();

	Matrix4x4 rotateMatrix = MakeRotateMatrix(camera_->GetCameraObj()->GetRotation());
	Vector3 linePos = Transform_(lineStartPos_, rotateMatrix);

	line_->SetPosition(
		camera_->GetCameraObj()->GetWorldPosition() + linePos,
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
