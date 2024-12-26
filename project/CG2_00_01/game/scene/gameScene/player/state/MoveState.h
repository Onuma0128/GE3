#pragma once

#include "Input.h"
#include "GlobalVariables.h"

#include "gameScene/player/state/BaseState.h"

#include "Quaternion.h"

class MoveState : public BaseState
{
public:

	MoveState(Player* player);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	float LerpShortAngle(float a, float b, float t);

private:

	Input* input_ = Input::GetInstance();
	GlobalVariables* global_ = GlobalVariables::GetInstance();

	Quaternion yRotation_ = Quaternion::IdentityQuaternion();
};