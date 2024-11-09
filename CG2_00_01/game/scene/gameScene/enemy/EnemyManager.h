#pragma once

#include <memory>
#include <vector>
#include <array>

#include "globalVariables/GlobalVariables.h"
#include "Input.h"
#include "Enemy.h"
#include "Object3d.h"

class RailCamera;

class EnemyManager
{
public:

	void Initialize();

	void PopEnemyInitialize();

	void Update();

	void Draw();

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

	std::unique_ptr<Object3d> box_;

};

