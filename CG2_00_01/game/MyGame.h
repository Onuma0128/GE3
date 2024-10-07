#pragma once
#include <dxgidebug.h>
#pragma comment(lib,"dxguid.lib")
#include "Framework.h"
#include "scene/GamePlayScene.h"

class MyGame : public Framework
{
public:

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

private:

	GamePlayScene* scene_ = nullptr;
};