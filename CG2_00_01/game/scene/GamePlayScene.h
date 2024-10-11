#pragma once
#include <memory>
#include <vector>
#include "wrl.h"

#include "SpriteBase.h"
#include "Sprite.h"
#include "Object3dBase.h"
#include "Object3d.h"
#include "LoadSound.h"
#include "BaseScene.h"
#include "Line3d.h"

using Microsoft::WRL::ComPtr;

class GamePlayScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	std::vector<std::unique_ptr<Sprite>> sprites_;
	std::vector<std::unique_ptr<Object3d>> obj_;
	std::vector<std::unique_ptr<Line3d>> lines_;

	ComPtr<IXAudio2> xAudio2;
	SoundData soundData1;
};

