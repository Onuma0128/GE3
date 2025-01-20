#pragma once

#include <memory>
#include <list>

#include "TrailEffect.h"
#include "GlobalVariables.h"

class EnemyEffect
{
public:

	struct HitEffect {
		std::unique_ptr<TrailEffect> effect_;
		Vector3 velocity_;
		float alpha_;
	};

	void Init();
	void GlobalInit();

	void Update();

	void Draw();

	std::list<HitEffect>& GetHitEffects() { return hitEffects_; }

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	std::list<HitEffect> hitEffects_;

};