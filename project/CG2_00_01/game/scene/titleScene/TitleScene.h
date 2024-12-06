#pragma once
#include <memory>
#include <vector>

#include "wrl.h"

#include "LoadSound.h"
#include "BaseScene.h"
#include "Camera.h"

#include "player/Player.h"

using Microsoft::WRL::ComPtr;

class TitleScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<Camera> camera1_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// 地面を配置
	std::unique_ptr<Object3d> map_;
	std::unique_ptr<ParticleEmitter> emitter_;

	// 今後消す
	ComPtr<IXAudio2> xAudio2;
	SoundData soundData1;
};

