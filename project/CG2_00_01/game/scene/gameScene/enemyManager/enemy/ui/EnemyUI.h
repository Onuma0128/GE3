#pragma once

#include <memory>

#include "Sprite.h"
#include "GlobalVariables.h"

class Enemy;

class EnemyUI
{
public:

	void Init();
	void GlobalInit();

	void Update();

	void Draw();

	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

private:

	Enemy* enemy_ = nullptr;
	GlobalVariables* global_ = GlobalVariables::GetInstance();

	std::unique_ptr<Sprite> hpSprite_ = nullptr;

};

