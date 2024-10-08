#pragma once
#include <memory>
#include <vector>
#include "wrl.h"

#include "LoadSound.h"
#include "Sprite.h"
#include "Object3d.h"
#include "BaseScene.h"

using Microsoft::WRL::ComPtr;

class TitleScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	std::vector<std::unique_ptr<Sprite>> sprites_;
	std::vector <std::unique_ptr<Object3d>> obj_;

	ComPtr<IXAudio2> xAudio2;
	SoundData soundData1;
};

