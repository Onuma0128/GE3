#pragma once
#include "scene/BaseScene.h"

class SceneManager
{
private:
	static SceneManager* instance_;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = delete;
	SceneManager& operator=(SceneManager&) = delete;

public:
	// シングルトンインスタンスの取得
	static SceneManager* GetInstance();

public:

	void Update();

	void Draw();

	void Finalize();

	// 次のシーン予約
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

private:
	// 今のシーン
	BaseScene* scene_ = nullptr;
	// 次のシーン
	BaseScene* nextScene_ = nullptr;
};