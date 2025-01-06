#pragma once
#include <memory>

#include "BaseScene.h"

#include "Sprite.h"
#include "Camera.h"

#include "gameScene/fade/FadeScene.h"

class GameOverScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	std::unique_ptr<Camera> camera_ = nullptr;

	std::unique_ptr<Sprite> sprite_ = nullptr;

	std::unique_ptr<FadeScene> fade_ = nullptr;
	bool isFade_ = false;
};

