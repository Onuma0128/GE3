#pragma once

#include <memory>
#include <vector>

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

};

