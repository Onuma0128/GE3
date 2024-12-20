#pragma once
#include <memory>
#include <array>
#include <vector>

#include "LoadSound.h"
#include "Sprite.h"
#include "Object3d.h"
#include "BaseScene.h"

#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "GlobalVariables.h"
#include "Camera.h"
#include "Audio.h"

class TitleScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<Camera> camera1_ = nullptr;

	std::vector<std::unique_ptr<Sprite>> sprites_;
	std::vector <std::unique_ptr<Object3d>> obj_;

	std::unique_ptr<ParticleEmitter> emitter_;
};

