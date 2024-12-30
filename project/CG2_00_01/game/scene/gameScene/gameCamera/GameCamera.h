#pragma once

#include <memory>

#include "Camera.h"
#include "GlobalVariables.h"

class Player;

class GameCamera
{
public:

	void Init();

	void GlobalInit();

	void Update();

	void SetPlayer(Player* player) { player_ = player; }

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	std::unique_ptr<Camera> camera_ = nullptr;

	Player* player_ = nullptr;

	float time_ = 0.0f;
	float duration_ = 0.5f;  // 振動の継続時間 (秒)
	float intensity_ = 0.2f; // 最大振幅
	float deltaTime_ = 1.0f / 60.0f; // シミュレーションの時間刻み
};

