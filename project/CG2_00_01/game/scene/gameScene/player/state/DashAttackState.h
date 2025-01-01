#pragma once

#include "Input.h"
#include "GlobalVariables.h"

#include "gameScene/player/state/BaseState.h"

class DashAttackState : public BaseState
{
public:

	DashAttackState(Player* player, PlayerAnimation* playerAnimation);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	Input* input_ = Input::GetInstance();
	GlobalVariables* global_ = GlobalVariables::GetInstance();
};