#pragma once

#include "GlobalVariables.h"

#include "Quaternion.h"
#include "gameScene/enemyManager/enemy/state/BaseStateEnemy.h"

class AttackStateEnemy :public BaseStateEnemy
{
public:

	AttackStateEnemy(Enemy* enemy);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	Quaternion offsetQuaternion_{};
	float attackFrame_ = 0.0f;

};