#pragma once

#include <memory>

#include "Input.h"
#include "GlobalVariables.h"
#include "Audio.h"

#include "gameScene/player/state/BaseState.h"
#include "ParticleEmitter.h"

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

	// 現在のコンボ
	AttackCombo nowCombo_ = AttackCombo::Combo1;

	// 攻撃Audio
	std::unique_ptr<Audio> attackAudio_ = nullptr;

};

