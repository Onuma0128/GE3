#pragma once

#include <memory>
#include <list>
#include <vector>

#include "TrailEffect.h"
#include "GlobalVariables.h"

class Player;

class PlayerEffect
{
public:

	struct SwordEffect {
		std::unique_ptr<TrailEffect> effect_;
		float alpha_;
	};


	void Init();

	void GlobalInit();

	void Update();

	void Draw();
	void DrawTrail();

	void SetPlayer(Player* player) { player_ = player; }

	std::list<SwordEffect>& GetTrailEffects() { return trailEffects_; }

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	Player* player_ = nullptr;

	// 剣のエフェクト
	std::list<SwordEffect> trailEffects_;

};

