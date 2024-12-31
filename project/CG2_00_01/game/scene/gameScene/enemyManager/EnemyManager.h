#pragma once

#include <memory>
#include <vector>

#include "enemy/Enemy.h"

class Player;

class EnemyManager
{
public:

	~EnemyManager();

	void Init();

	void GlobalInit();

	void Update();

	void Draw();

	void Debug_ImGui();

	void CollisionEnemyToEnemy(Enemy* enemy);

	// 敵のポインタを取得
	std::vector<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }

	void SetPlayer(Player* player) { player_ = player; }

private:

	void AddEnemy(int count);

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	Player* player_ = nullptr;

	std::vector<std::unique_ptr<Enemy>> enemys_;

};

