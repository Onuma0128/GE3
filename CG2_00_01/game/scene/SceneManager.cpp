#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new SceneManager;
	}
	return instance_;
}

void SceneManager::Update()
{
	if (nextScene_) {
		// 旧シーンの終了
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}

		// シーン切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;
		// 次シーンを初期化する
		scene_->Initialize();
	}

	// 実行中のシーンの更新
	scene_->Update();
}

void SceneManager::Draw()
{
	// 実行中のシーンの描画
	scene_->Draw();
}

void SceneManager::Finalize()
{
	scene_->Finalize();
	delete scene_;

	delete instance_;
	instance_ = nullptr;
}
