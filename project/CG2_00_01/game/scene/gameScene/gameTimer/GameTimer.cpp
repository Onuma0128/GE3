#include "GameTimer.h"

#include <cmath>

#include "GlobalVariables.h"

GameTimer* GameTimer::instance_ = nullptr;

GameTimer* GameTimer::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new GameTimer;
	}
	return instance_;
}

void GameTimer::Init()
{
	GlobalInit();


}

void GameTimer::GlobalInit()
{
	GlobalVariables::GetInstance()->AddValue<float>("GameTimer", "HitStopTime", 10.0f);
	GlobalVariables::GetInstance()->AddValue<float>("GameTimer", "deltaLerpSpeed", 0.9f);
}

void GameTimer::Updata()
{
	// フラグが立ったらdeltaTimeを0にする
	if (isHitStop_) {
		deltaTime_ = 0.0f;
		hitStopTime_ += 1.0f / GlobalVariables::GetInstance()->GetValue<float>("GameTimer", "HitStopTime");
		if (hitStopTime_ >= 1.0f) {
			isHitStop_ = false;
		}
	}

	// deltaTimeが1以下になったら1に戻す
	if (deltaTime_ < 1.0f && !isHitStop_) {
		deltaTime_ = std::lerp(deltaTime_, 1.0f, GlobalVariables::GetInstance()->GetValue<float>("GameTimer", "deltaLerpSpeed"));
	}
	else if (!isHitStop_) {
		deltaTime_ = 1.0f;

		hitStopTime_ = 0.0f;
		isHitStop_ = false;
	}

}
