#pragma once

#include <memory>
#include <vector>
#include <array>

#include "globalVariables/GlobalVariables.h"
#include "Input.h"
#include "Enemy.h"
#include "Object3d.h"
#include "Sprite.h"

class RailCamera;

class EnemyManager
{
public:

	void Initialize();

	void Update();

	void SpriteUpdate();

	void Draw();

	void DrawSprite();

	void CreateEnemy();

	const std::vector<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }

	void SetRailCamera(RailCamera* camera) { camera_ = camera; }

private:

	std::vector<std::unique_ptr<Enemy>> enemys_;
	std::array<bool, 5> popEnemy_ = { false,false,false,false,false };
	std::array<bool, 5> popEnemyTriggered_ = { false,false,false,false,false };

	GlobalVariables* global = GlobalVariables::GetInstance();

	Input* input_ = Input::GetInstance();

	RailCamera* camera_ = nullptr;

	std::vector<std::unique_ptr<Sprite>> sprites_;
	int gameScore_ = 0;

};

