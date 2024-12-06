#pragma once

#include <memory>

#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "Object3d.h"

#include "GlobalVariables.h"
#include "Vector3.h"

#include "titleScene/player/state/PlayerBase.h"

class Player
{
public:

	void Initialize();

	void GlobalInitialize();

	void Update();

	void Draw();

	Object3d* GetPlayerObj()const { return model_.get(); }

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	std::unique_ptr<Object3d> model_ = nullptr;
	std::unique_ptr<PlayerBase> state_ = nullptr;

	// プレイヤーのパーティクル
	ParticleManager* particleManager_ = ParticleManager::GetInstance();
	std::unique_ptr<ParticleEmitter> emitter_ = nullptr;

	// 炎のパーティクル
	std::unique_ptr<ParticleEmitter> emitterbackFlame_;
	std::unique_ptr<ParticleEmitter> emitterFlame_;

};