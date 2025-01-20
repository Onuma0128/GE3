#pragma once

#include "GlobalVariables.h"

#include "gameScene/enemyManager/enemy/state/BaseStateEnemy.h"

class DamageStateEnemy : public BaseStateEnemy
{
public:

	DamageStateEnemy(Enemy* enemy);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	bool isDamage_ = false;
	float damageFrame_ = 0.0f;
	float velocityY = 1.0f;
};

