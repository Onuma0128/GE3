#pragma once

#include <memory>
#include <list>

#include "Object3d.h"
#include "WorldTransform.h"
#include "ParticleEmitter.h"
#include "GlobalVariables.h"

class Player;

class PlayerParticle
{
public:

	struct Combo3Particle {
		std::unique_ptr<Object3d> model_;
		std::unique_ptr<WorldTransform> transform_;
		Vector3 velocity_;
		float alpha_;
	};

	void Init();

	void GlobalInit();

	void Update();

	void Draw();

	void CreateParticle(const Vector3& position);

	void SetPlayer(Player* player) { player_ = player; }

	ParticleEmitter* GetMoveEmitter()const { return moveParticleEmitter_.get(); }
	ParticleEmitter* GetSwordEmitter()const { return swordParticleEmitter_.get(); }

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	Player* player_ = nullptr;

	// 移動時のパーティクル
	std::unique_ptr<ParticleEmitter> moveParticleEmitter_ = nullptr;
	// 攻撃時剣先のパーティクル
	std::unique_ptr<ParticleEmitter> swordParticleEmitter_ = nullptr;

	// 3コンボ目のパーティクル
	std::list<Combo3Particle> combo3Particles_;
	std::unique_ptr<Object3d> model_ = nullptr;
	std::unique_ptr<WorldTransform> transform_ = nullptr;
	float alpha_ = 1.0f;

};
