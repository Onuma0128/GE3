#pragma once
#include <memory>
#include <vector>
#include "wrl.h"

#include "SpriteBase.h"
#include "Sprite.h"
#include "Object3dBase.h"
#include "Object3d.h"
#include "LoadSound.h"

using Microsoft::WRL::ComPtr;

class GamePlayScene
{

public:

	void Initialize();

	void Finalize();

	void Update();

	void Draw();

private:

	std::vector<std::unique_ptr<Sprite>> sprites_;
	std::vector <std::unique_ptr<Object3d>> obj_;

	ComPtr<IXAudio2> xAudio2;
	SoundData soundData1;
};

