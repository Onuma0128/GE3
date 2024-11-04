#pragma once

#include <memory>
#include <vector>

#include "globalVariables/GlobalVariables.h"
#include "Input.h"
#include "Enemy.h"

class EnemyManager
{
public:

	void Initialize();

	void Update();

	void Draw();

	const std::vector<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }

private:

	std::vector<std::unique_ptr<Enemy>> enemys_;

	GlobalVariables* global = GlobalVariables::GetInstance();

	Input* input_ = Input::GetInstance();

};

