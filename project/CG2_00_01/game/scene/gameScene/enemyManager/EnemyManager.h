#pragma once

#include <memory>
#include <vector>

#include "enemy/Enemy.h"
#include "gameScene/fade/FadeScene.h"

class Player;

class EnemyManager
{
public:

	~EnemyManager();

	void Init();

	void GlobalInit();

	void Update();

	void Draw();
	void DrawSprite();

	void Debug_ImGui();

	// 敵のポインタを取得
	std::vector<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }

	void SetPlayer(Player* player) { player_ = player; }

private:

	void AddEnemy(int count);

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	Player* player_ = nullptr;

	std::vector<std::unique_ptr<Enemy>> enemys_;
	std::unique_ptr<FadeScene> fade_ = nullptr;

};

