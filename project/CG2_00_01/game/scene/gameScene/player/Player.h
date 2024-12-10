#pragma once
#include <memory>
#include <list>

#include "Input.h"
#include "Object3d.h"
#include "GlobalVariables.h"
#include "ParticleEmitter.h"

#include "Vector3.h"

#include "attackParticle/AttackParticle.h"

class Player
{
public:

	enum class State {
		Move,
		Jump,
		Attack
	};

	void Init();

	void Update();

	void Draw();

	void SetIsShake(bool isShake) { isShake_ = isShake; }

	bool GetIsShake()const { return isShake_; }

private:

	void GlobalInit();

	void Move();

	void Jump();

	void Attack();

	float LerpShortAngle(float a, float b, float t);

private:

	Input* input_ = Input::GetInstance();
	GlobalVariables* global_ = GlobalVariables::GetInstance();

	// モデル
	std::unique_ptr<Object3d> model_ = nullptr;
	std::unique_ptr<Object3d> shadowModel_ = nullptr;
	// パーティクル
	std::unique_ptr<ParticleEmitter> emitter_ = nullptr;
	// 攻撃時のパーティクル
	std::list<std::unique_ptr<AttackParticle>> attackParticles_;

	// 状態
	State state_ = State::Move;

	// 変数
	Vector3 velocity_;
	Vector3 acceleration_;
	float attackAnimaFrame_;
	bool isShake_ = false;

};