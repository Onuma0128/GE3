#include "EnemyEffect.h"

#include "Transform.h"

void EnemyEffect::Init()
{
	GlobalInit();

	for (int i = 0; i < 20; ++i) {
		HitEffect hitEffect;
		hitEffect.effect_ = std::make_unique<TrailEffect>();
		hitEffect.effect_->InitSphere(8);
		hitEffect.alpha_ = 0.0f;
		hitEffects_.push_back(std::move(hitEffect));
	}
}

void EnemyEffect::GlobalInit()
{
	global_->AddValue<int>("EnemyHitEffect", "count", 10);
	global_->AddValue<float>("EnemyHitEffect", "velocityPow", 0.01f);
	global_->AddValue<Vector3>("EnemyHitEffect", "scale", Vector3{ 0.1f,0.3f,0.1f });
	global_->AddValue<Vector3>("EnemyHitEffect", "color", Vector3{ 1.0f,1.0f,1.0f });
	global_->AddValue<float>("EnemyHitEffect", "alpha", 0.01f);
}

void EnemyEffect::Update()
{
	for (auto& effect : hitEffects_) {
		if (effect.alpha_ > 0.0f) {
			Transform transform = effect.effect_->GetTransform();
			transform.translate += effect.velocity_;
			effect.effect_->SetTransform(transform);

			effect.effect_->SetColor(global_->GetValue<Vector3>("EnemyHitEffect", "color"));
			effect.alpha_ -= global_->GetValue<float>("EnemyHitEffect", "alpha");
			effect.effect_->SetAlpha(effect.alpha_);
		}
		else {
			effect.alpha_ = 0.0f;
			effect.effect_->SetAlpha(effect.alpha_);
		}
		effect.effect_->Update();
	}
}

void EnemyEffect::Draw()
{
	for (auto& effect : hitEffects_) {
		if (effect.alpha_ > 0.0f) {
			effect.effect_->DrawSphere();
		}
	}
}
