#include "Input.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input* Input::instance_ = nullptr;

Input* Input::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new Input;
	}
	return instance_;
}

void Input::Initialize(WinApp* winApp)
{
	winApp_ = winApp;

	HRESULT hr{};

	// 前回のキー入力を保存
	memcpy(keyPre_, key_, sizeof(key_));

	hr = DirectInput8Create(
		winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput_, nullptr);
	assert(SUCCEEDED(hr));


	// キーボードデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(hr));
	// 入力データ形式のセット
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));
	// 排他制御レベルのセット
	hr = keyboard_->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

	// マウスデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(hr));
	// 入力データ形式のセット
	hr = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(hr));
	// 排他制御レベルのセット
	hr = mouse_->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

	mouse_->Acquire();
}

void Input::Update()
{
	// 前回のキー入力を保存
	memcpy(keyPre_, key_, sizeof(key_));
	// キーボード情報の取得開始
	keyboard_->Acquire();
	// 全キーの入力状態を取得する
	keyboard_->GetDeviceState(sizeof(key_), key_);

	// マウス情報の取得開始
	mouse_->Acquire();
	HRESULT hr = mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState_);
	if (FAILED(hr)) {
		// デバイスが一時的に使用できない場合は再取得
		mouse_->Acquire();
	}

	// マウスの移動量とボタンの状態を取得
	mouseDeltaX_ = mouseState_.lX;
	mouseDeltaY_ = mouseState_.lY;
	POINT cursorPos;
	if (GetCursorPos(&cursorPos)) {
		// クライアント座標系に変換
		ScreenToClient(winApp_->GetHwnd(), &cursorPos);
		mousePosX_ = cursorPos.x;
		mousePosY_ = cursorPos.y;
	}

	memcpy(mouseButtonPre_, mouseButton_, sizeof(mouseButton_));
	memcpy(mouseButton_, mouseState_.rgbButtons, sizeof(mouseButton_));
}

bool Input::PushKey(BYTE keyNumber)
{
	// 指定キーが押していればtrueを返す
	return key_[keyNumber] != 0;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 指定キーが現在押されていて、前回は押されていなかった場合にtrueを返す
	return key_[keyNumber] && !keyPre_[keyNumber];
}

bool Input::PushMouseButton(int buttonNumber) const
{
	return (mouseButton_[buttonNumber] & 0x80) != 0;
}

bool Input::TriggerMouseButton(int buttonNumber) const
{
	return (mouseButton_[buttonNumber] & 0x80) != 0 && !(mouseButtonPre_[buttonNumber] & 0x80);
}

void Input::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}