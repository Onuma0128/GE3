#pragma once
#include <memory>
#include <list>

#include "Input.h"
#include "Object3d.h"
#include "GlobalVariables.h"
#include "ParticleEmitter.h"

#include "Vector3.h"

#include "state/BaseState.h"
#include "attackParticle/AttackParticle.h"

class Player
{
public:

	~Player();

	void Init();

	void Update();

	void Draw();

	void SetIsShake(bool isShake) { isShake_ = isShake; }
	bool GetIsShake()const { return isShake_; }

	/* ==================== モデル ==================== */

	Object3d* GetModel()const { return model_.get(); }
	Object3d* GetShadowModel()const { return shadowModel_.get(); }

	/* ==================== パーティクル ==================== */

	ParticleEmitter* GetMoveEmitter()const { return moveParticleEmitter_.get(); }
	void SetAttackParticle(std::unique_ptr<AttackParticle> particle) { attackParticles_.push_back(std::move(particle)); }

	/* ==================== playerの変数 ==================== */

	const Vector3& GetVelocity() { return velocity_; }
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	const Vector3& GetAcceleration() { return acceleration_; }
	void SetAcceleration(const Vector3& acceleration) { acceleration_ = acceleration; }

	void ChengeState(std::unique_ptr<BaseState> newState);

private:

	void GlobalInit();

private:

	Input* input_ = Input::GetInstance();
	GlobalVariables* global_ = GlobalVariables::GetInstance();

	// モデル
	std::unique_ptr<Object3d> model_ = nullptr;
	std::unique_ptr<Object3d> shadowModel_ = nullptr;

	// 移動時のパーティクル
	std::unique_ptr<ParticleEmitter> moveParticleEmitter_ = nullptr;
	// 攻撃時のパーティクル
	std::list<std::unique_ptr<AttackParticle>> attackParticles_;

	// 状態
	std::unique_ptr<BaseState> state_;

	// 変数
	Vector3 velocity_;
	Vector3 acceleration_;
	bool isShake_ = false;

};