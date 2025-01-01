#pragma once

#include "Input.h"
#include "GlobalVariables.h"

#include "gameScene/player/state/BaseState.h"

class DamageState : public BaseState
{
public:

	DamageState(Player* player, PlayerAnimation* playerAnimation);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	Input* input_ = Input::GetInstance();
	GlobalVariables* global_ = GlobalVariables::GetInstance();

	float knockbackFrame_ = 0.0f;
};

