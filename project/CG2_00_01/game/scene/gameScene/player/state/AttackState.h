#pragma once

#include "Input.h"
#include "GlobalVariables.h"

#include "gameScene/player/state/BaseState.h"

class AttackState : public BaseState
{
public:

	enum class AttackCombo {
		Combo1,
		Combo2,
		Combo3
	};

	AttackState(Player* player, PlayerAnimation* playerAnimation);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	Input* input_ = Input::GetInstance();
	GlobalVariables* global_ = GlobalVariables::GetInstance();


	AttackCombo nowCombo_ = AttackCombo::Combo1;
};

