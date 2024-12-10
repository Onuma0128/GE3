#pragma once
#include <memory>

#include "Input.h"
#include "Object3d.h"
#include "GlobalVariables.h"

#include "Vector3.h"

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

	// 状態
	State state_ = State::Move;

	// 変数
	Vector3 velocity_;
	Vector3 acceleration_;
	float attackAnimaFrame_;

};