#pragma once

#include <memory>
#include <vector>

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

	void CreateSwordEffect(const Vector3& pos1, const Vector3& pos2);

	void CreateDashEffect();

private:

	Input* input_ = Input::GetInstance();
	GlobalVariables* global_ = GlobalVariables::GetInstance();

	Vector3 velocity_;

	// トレイル描画する座標を格納
	std::vector<Vector3> trailPositions_;

	bool isCreateEffect_ = false;
};