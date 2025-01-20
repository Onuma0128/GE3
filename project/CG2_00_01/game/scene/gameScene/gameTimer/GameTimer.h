#pragma once
class GameTimer
{
private:
	static GameTimer* instance_;

	GameTimer() = default;
	~GameTimer() = default;
	GameTimer(GameTimer&) = delete;
	GameTimer& operator=(GameTimer&) = delete;

public:
	// シングルトンインスタンスの取得
	static GameTimer* GetInstance();

	void Init();
	void GlobalInit();
	void Updata();

	float GetDeltaTime() const { return deltaTime_; }
	void SetHitStop(bool flag) { 
		isHitStop_ = flag;
		deltaTime_ = 0.0f;
		hitStopTime_ = 0.0f;
	}

private:

	float deltaTime_ = 1.0f;

	float hitStopTime_ = 0.0f;
	bool isHitStop_ = false;

};
