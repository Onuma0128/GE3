#pragma once
#include <dxgidebug.h>
#pragma comment(lib,"dxguid.lib")
#include "Framework.h"

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

	void LoadResource();

private:

};