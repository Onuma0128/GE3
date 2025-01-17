#include "PlayerEffect.h"

#include "gameScene/player/Player.h"

void PlayerEffect::Init()
{
	GlobalInit();
}

void PlayerEffect::GlobalInit()
{
	global_->AddValue<float>("PlayerTrailEffect", "alpha", 0.1f);
	global_->AddValue<Vector3>("PlayerTrailEffect", "color", Vector3{ 1.0f,1.0f,1.0f });
}

void PlayerEffect::Update()
{
	for (auto& trail : trailEffects_) {
		trail.alpha_ -= global_->GetValue<float>("PlayerTrailEffect", "alpha");
		trail.effect_->SetColor(global_->GetValue<Vector3>("PlayerTrailEffect", "color"));
		trail.effect_->SetAlpha(trail.alpha_);
		trail.effect_->Update();
	}
	for (auto it = trailEffects_.begin(); it != trailEffects_.end();) {
		if ((it)->alpha_ <= 0.0f) {
			it = trailEffects_.erase(it);
		}
		else {
			++it;
		}
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
