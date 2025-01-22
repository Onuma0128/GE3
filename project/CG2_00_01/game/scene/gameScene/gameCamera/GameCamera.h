#pragma once

#include <memory>

#include "Camera.h"
#include "GlobalVariables.h"

class Player;

class GameCamera
{
public:

	struct Shake {
		float time_;
		float duration_; 
		float intensity_; 
		float deltaTime_;
	};

	void Init();

	void GlobalInit();

	void Update();

	void SetPlayer(Player* player) { player_ = player; }

	float LerpShortAngle(float a, float b, float t);

	void SetIsShake(bool flag) { isShake_ = flag; }

	Camera* GetCamera() { return camera_.get(); }

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	std::unique_ptr<Camera> camera_ = nullptr;
	Vector3 offset_{};
	float destinationAngleY = 0.0f;

	Player* player_ = nullptr;

	bool isShake_ = false;
	Shake hitStopShake_;

	Shake playerDamageShake_;
};

