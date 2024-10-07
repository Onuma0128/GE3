#pragma once
#include <dxgidebug.h>
#pragma comment(lib,"dxguid.lib")
#include <memory>
#include <vector>
#include "wrl.h"
#include "Input.h"
#include "LoadSound.h"
#include "DirectXEngine.h"
#include "SpriteBase.h"
#include "Sprite.h"
#include "Object3dBase.h"
#include "Object3d.h"
#include "LightManager.h"
#include "Camera.h"

class MyGame
{
public:

	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 更新
	void Update();

	// 描画
	void Draw();


	bool isEndRequst() { return endRequst_; }

private:
	// ゲーム終了フラグ
	bool endRequst_ = false;

	WinApp* winApp_ = nullptr;
	DirectXEngine* directXEngine_ = nullptr;
	Input* input_ = nullptr;

	std::vector<Sprite*> sprites_;
	std::vector<Object3d*> obj_;

	ComPtr<IXAudio2> xAudio2;
	SoundData soundData1;
};