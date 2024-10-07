#pragma once
#include "wrl.h"
#include <memory>

#include "WinApp.h"
#include "DirectXEngine.h"
#include "Input.h"
#include "LoadSound.h"

class Framework
{
public:

	virtual ~Framework() = default;

	virtual void Initialize();

	virtual void Finalize();

	virtual void Update();

	virtual void Draw() = 0;

	virtual bool IsEndRequst() { return endRequst_; }

	void Run();

protected:

	// ゲーム終了フラグ
	bool endRequst_ = false;

	std::unique_ptr<WinApp> winApp_ = nullptr;
	std::unique_ptr<DirectXEngine> directXEngine_ = nullptr;
	std::unique_ptr<Input> input_ = nullptr;

	ComPtr<IXAudio2> xAudio2;
	SoundData soundData1;
};