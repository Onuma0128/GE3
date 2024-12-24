#include "AttackParticle.h"

#include "random"

void AttackParticle::Init(const std::string& modelName, const Vector3& position)
{
	transform_ = std::make_unique<WorldTransform>();
	particle_ = std::make_unique<Object3d>();
	particle_->Initialize(modelName, transform_.get());
	float scale = global_->GetValue<float>("PlayerAttackParticle", "scale");
	transform_->scale_ = Vector3{ scale,scale,scale };
	Vector3 offset = global_->GetValue<Vector3>("PlayerAttackParticle", "offset");
	transform_->translation_ = position + offset;
	particle_->SetTexture("resources", "white1x1.png");
	float velocityPow = global_->GetValue<float>("PlayerAttackParticle", "velocityPow");
	velocity_ = {
		static_cast<float>(rand() % 11 - 5) * velocityPow,
		0.1f,
		static_cast<float>(rand() % 11 - 5) * velocityPow,
	};

	isActive_ = true;
	alpha_ = 1.0f;
}

void AttackParticle::Update()
{
	if (!isActive_) {
		return;
	}

	if (transform_->translation_.y < 0.0f && velocity_.y < 0.0f) {
		velocity_.y = -1.0f;
		alpha_ -= global_->GetValue<float>("PlayerAttackParticle", "alphaSubtrac");
		particle_->SetColor(Vector4{ 1.0f,1.0f,1.0f,alpha_ });
		if (alpha_ <= 0.0f) {
			isActive_ = false;
		}
	}
	else {
		velocity_.y -= 0.005f;
		transform_->rotation_ += velocity_;
		transform_->translation_ += velocity_;
	}
	particle_->Update();
}

void AttackParticle::Draw()
{
	particle_->Draw();
}
