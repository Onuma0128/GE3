#pragma once
#include <memory>

#include "BaseScene.h"

#include "Camera.h"
#include "gameScene/fade/FadeScene.h"

class TitleScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	std::unique_ptr<Camera> camera_ = nullptr;

	std::unique_ptr<FadeScene> fade_ = nullptr;
	bool isFade_ = false;

};

