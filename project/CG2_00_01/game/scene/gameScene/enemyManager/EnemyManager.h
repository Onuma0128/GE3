#pragma once

#include <memory>
#include <vector>

#include "enemy/Enemy.h"
#include "gameScene/fade/FadeScene.h"

class Player;
class GameCamera;

class EnemyManager
{
public:

	~EnemyManager();

	void Init();

	void GlobalInit();

	void Update();

	void Draw();
	void DrawSprite();
	void DrawEffect();

	void Debug_ImGui();

	// 敵のポインタを取得
	std::vector<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }

	void SetPlayer(Player* player) { player_ = player; }
	void SetGameCamera(GameCamera* camera) { camera_ = camera; }

private:

	void AddEnemy(int count);

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	Player* player_ = nullptr;
	GameCamera* camera_ = nullptr;

	std::vector<std::unique_ptr<Enemy>> enemys_;
	std::unique_ptr<FadeScene> fade_ = nullptr;

};

