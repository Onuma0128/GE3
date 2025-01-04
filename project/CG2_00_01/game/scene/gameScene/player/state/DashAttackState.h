#pragma once

#include <memory>

#include "Input.h"
#include "GlobalVariables.h"
#include "Audio.h"

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

	Vector3 velocity_;

	// 攻撃Audio
	std::unique_ptr<Audio> attackAudio_ = nullptr;
};