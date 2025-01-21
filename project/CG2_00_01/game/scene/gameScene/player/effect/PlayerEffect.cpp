#include "PlayerEffect.h"

#include "imgui.h"
#include "gameScene/gameTimer/GameTimer.h"
#include "gameScene/player/Player.h"

void PlayerEffect::Init()
{
	GlobalInit();

	// トレイルエフェクトの初期化
	std::vector<Vector3> pos{ Vector3{},Vector3{} ,Vector3{} ,Vector3{} };
	for (int i = 0; i < 70; ++i) {
		SwordEffect trail;
		trail.effect_ = std::make_unique<TrailEffect>();
		trail.effect_->Init(pos);
		trail.alpha_ = 0.0f;
		trailEffects_.push_back(std::move(trail));
	}

	// ダッシュエフェクトの初期化
	for (int i = 0; i < 30; ++i) {
		DashEffect dash;
		dash.effect_ = std::make_unique<TrailEffect>();
		dash.effect_->InitSphere(8);
		dash.velocity_ = Vector3{};
		dash.alpha_ = 0.0f;
		dashEffects_.push_back(std::move(dash));
	}

}

void PlayerEffect::GlobalInit()
{
	global_->AddValue<float>("PlayerTrailEffect", "alpha", 0.1f);
	global_->AddValue<Vector3>("PlayerTrailEffect", "color", Vector3{ 1.0f,1.0f,1.0f });

	global_->AddValue<int>("PlayerDashEffect", "count", 10);
	global_->AddValue<float>("PlayerDashEffect", "alpha", 0.1f);
	global_->AddValue<float>("PlayerDashEffect", "velocityPow", 0.1f);
	global_->AddValue<float>("PlayerDashEffect", "velocitySize", 5.0f);
	global_->AddValue<Vector3>("PlayerDashEffect", "color", Vector3{ 1.0f,1.0f,1.0f });
	global_->AddValue<Vector3>("PlayerDashEffect", "scale", Vector3{ 1.0f,1.0f,1.0f });
}

void PlayerEffect::Update()
{
	for (auto& trail : trailEffects_) {
		if (trail.alpha_ > 0.0f) {
			trail.alpha_ -= global_->GetValue<float>("PlayerTrailEffect", "alpha") * GameTimer::GetInstance()->GetDeltaTime();
			trail.effect_->SetColor(global_->GetValue<Vector3>("PlayerTrailEffect", "color"));
			trail.effect_->SetAlpha(trail.alpha_);
		}
		else {
			trail.alpha_ = 0.0f;
			trail.effect_->SetAlpha(trail.alpha_);
		}
		trail.effect_->Update();
	}

	for (auto& dash : dashEffects_) {
		if (dash.alpha_ > 0.0f) {
			Transform transform = dash.effect_->GetTransform();
			transform.translate += dash.velocity_ * GameTimer::GetInstance()->GetDeltaTime() *
				global_->GetValue<float>("PlayerDashEffect", "velocityPow");
			dash.effect_->SetTransform(transform);

			dash.alpha_ -= global_->GetValue<float>("PlayerDashEffect", "alpha") * GameTimer::GetInstance()->GetDeltaTime();
			dash.effect_->SetColor(global_->GetValue<Vector3>("PlayerDashEffect", "color"));
			dash.effect_->SetAlpha(dash.alpha_);
		}
		else {
			dash.alpha_ = 0.0f;
			dash.effect_->SetAlpha(dash.alpha_);
		}
		dash.effect_->Update();
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

	for (auto& dash : dashEffects_) {
		dash.effect_->DrawSphere();
	}
}
