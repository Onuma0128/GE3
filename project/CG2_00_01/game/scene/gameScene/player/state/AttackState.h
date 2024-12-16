#pragma once

#include "Input.h"
#include "GlobalVariables.h"

#include "gameScene/player/state/BaseState.h"

class AttackState : public BaseState
{
public:

	AttackState(Player* player);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	Input* input_ = Input::GetInstance();
	GlobalVariables* global_ = GlobalVariables::GetInstance();


	float attackAnimaFrame_;
};