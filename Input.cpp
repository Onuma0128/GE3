#include "Input.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input::~Input()
{
	//delete winApp_;
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
}

void Input::Update()
{
	// 前回のキー入力を保存
	memcpy(keyPre_, key_, sizeof(key_));
	// キーボード情報の取得開始
	keyboard_->Acquire();
	// 全キーの入力状態を取得する
	keyboard_->GetDeviceState(sizeof(key_), key_);
}

bool Input::PushKey(BYTE keyNumber)
{
	// 指定キーが押していればtrueを返す
	if (key_[keyNumber]) {
		return true;
	}
	// そうでなければfalseを返す
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 指定キーが現在押されていて、前回は押されていなかった場合にtrueを返す
	if (key_[keyNumber] && !keyPre_[keyNumber]) {
		return true;
	}
	// そうでなければfalseを返す
	return false;
}
