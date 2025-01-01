#pragma once

#include "GlobalVariables.h"

#include "gameScene/enemyManager/enemy/state/BaseStateEnemy.h"

class MoveStateEnemy : public BaseStateEnemy
{
public:

	MoveStateEnemy(Enemy* enemy);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

};

