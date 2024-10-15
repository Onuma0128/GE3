#pragma once
#include <wrl.h>
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#include <cassert>
#include <windows.h>
#include "WinApp.h"

class Input
{
private:
	static Input* instance_;

	Input() = default;
	~Input() = default;
	Input(Input&) = delete;
	Input& operator=(Input&) = delete;

public:
	// シングルトンインスタンスの取得
	static Input* GetInstance();
public:

	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	// 初期化
	void Initialize(WinApp* winApp);
	// 更新
	void Update();
	
	// キーの押下をチェック
	bool PushKey(BYTE keyNumber);
	// キーのトリガーをチェック
	bool TriggerKey(BYTE keyNumber);
	// マウスボタンの押下
	bool PushMouseButton(int buttonNumber)const;
	// マウスボタンのトリガーをチェック
	bool TriggerMouseButton(int buttonNumber)const;
	// マウス座標の取得
	int GetMouseDeltaX()const { return mouseDeltaX_; }
	int GetMouseDeltaY()const { return mouseDeltaY_; }

	// 終了
	void Finalize();

private:
	ComPtr<IDirectInput8> directInput_;
	// キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard_;
	// マウスのデバイス
	ComPtr<IDirectInputDevice8> mouse_;

	// 全キーの状態
	BYTE key_[256] = {};
	// 前回の全キーの状態
	BYTE keyPre_[256] = {};
	// マウスの状態
	DIMOUSESTATE mouseState_ = {};
	// マウスボタンの状態
	BYTE mouseButton_[4] = {};
	// マウスボタンの前回の状態
	BYTE mouseButtonPre_[4] = {};

	// マウスの移動量
	int mouseDeltaX_ = 0;
	int mouseDeltaY_ = 0;

	// WindowsAPI
	WinApp* winApp_ = nullptr;
};