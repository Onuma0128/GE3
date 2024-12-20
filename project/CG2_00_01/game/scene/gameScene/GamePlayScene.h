#pragma once
#include <memory>

#include "BaseScene.h"

#include "Object3d.h"
#include "Camera.h"

class GamePlayScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	std::unique_ptr<Camera> camera_ = nullptr;

	std::unique_ptr<Object3d> model_ = nullptr;
};

