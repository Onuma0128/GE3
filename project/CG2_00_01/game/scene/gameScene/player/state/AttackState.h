#pragma once

#include <memory>
#include <vector>

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

	void CreateSwordEffect(const Vector3& pos1, const Vector3& pos2);

private:

	Input* input_ = Input::GetInstance();
	GlobalVariables* global_ = GlobalVariables::GetInstance();

	// 現在のコンボ
	AttackCombo nowCombo_ = AttackCombo::Combo1;


	// トレイル描画する座標を格納
	std::vector<Vector3> trailPositions_;

};

