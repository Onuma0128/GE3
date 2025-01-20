#include "PlayerEffect.h"

#include "gameScene/player/Player.h"

void PlayerEffect::Init()
{
	GlobalInit();

	std::vector<Vector3> pos{ Vector3{},Vector3{} ,Vector3{} ,Vector3{} };
	for (int i = 0; i < 40; ++i) {
		SwordEffect trail;
		trail.effect_ = std::make_unique<TrailEffect>();
		trail.effect_->Init(pos);
		trail.alpha_ = 0.0f;
		trailEffects_.push_back(std::move(trail));
	}
}

void PlayerEffect::GlobalInit()
{
	global_->AddValue<float>("PlayerTrailEffect", "alpha", 0.1f);
	global_->AddValue<Vector3>("PlayerTrailEffect", "color", Vector3{ 1.0f,1.0f,1.0f });
}

void PlayerEffect::Update()
{
	for (auto& trail : trailEffects_) {
		if (trail.alpha_ > 0.0f) {
			trail.alpha_ -= global_->GetValue<float>("PlayerTrailEffect", "alpha");
			trail.effect_->SetColor(global_->GetValue<Vector3>("PlayerTrailEffect", "color"));
			trail.effect_->SetAlpha(trail.alpha_);
		}
		else {
			trail.alpha_ = 0.0f;
			trail.effect_->SetAlpha(trail.alpha_);
		}
		trail.effect_->Update();
	}
}

void PlayerEffect::Draw()
{
}

void PlayerEffect::DrawTrail()
{
	for (auto& trail : trailEffects_) {
		trail.effect_->Draw();
	}
}
