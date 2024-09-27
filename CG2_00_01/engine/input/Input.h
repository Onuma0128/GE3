#pragma once
#include <wrl.h>
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#include <cassert>
#include <windows.h>
#include "WinApp.h"

class Input
{
public:
	~Input();

	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	// 初期化
	void Initialize(WinApp* winApp);
	// 更新
	void Update();
	// キーの押下をチェック
	bool PushKey(BYTE keyNumber);
	// キーのトリガーをチェック
	bool TriggerKey(BYTE keyNumber);

private:
	ComPtr<IDirectInput8> directInput_;
	// キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard_;
	// 全キーの状態
	BYTE key_[256] = {};
	// 前回の全キーの状態
	BYTE keyPre_[256] = {};
	// WindowsAPI
	WinApp* winApp_ = nullptr;
};