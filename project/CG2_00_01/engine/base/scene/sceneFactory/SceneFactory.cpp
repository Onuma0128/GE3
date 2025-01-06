#include "SceneFactory.h"

#include "titleScene/TitleScene.h"
#include "gameScene/GamePlayScene.h"
#include "clearScene/ClearScene.h"
#include "gameOverScene/GameOverScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	// 次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "Title") {
		newScene = new TitleScene();
	}
	else if (sceneName == "Game") {
		newScene = new GamePlayScene();
	}
	else if (sceneName == "Clear") {
		newScene = new ClearScene();
	}
	else if (sceneName == "GameOver") {
		newScene = new GameOverScene();
	}

	return newScene;
}
