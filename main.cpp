#include <dxgidebug.h>
#pragma comment(lib,"dxguid.lib")
#include <memory>
#include "wrl.h"
#include "Input.h"
#include "LoadSound.h"
#include "DirectXEngine.h"
#include "SpriteBase.h"
#include "Sprite.h"
#include "TextureManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	std::unique_ptr<WinApp>winApp_ = std::make_unique<WinApp>();
	winApp_->Initialize();

	std::unique_ptr<DirectXEngine> directXEngine_ = std::make_unique<DirectXEngine>();
	directXEngine_->Initialize(winApp_.get());

	std::unique_ptr<Input> input_ = std::make_unique<Input>();
	input_->Initialize(winApp_.get());

	SpriteBase::GetInstance()->Initialize(directXEngine_.get());

	TextureManager::GetInstance()->Initialize(directXEngine_.get());

	std::unique_ptr<Sprite> sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("uvChecker.png");
	sprite_->SetPosition({ 640,360 });
	sprite_->SetSize({ 256,256 });
	sprite_->SetAnchorPoint({ 0.5f,0.5f });

	// オーディオ
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	// XAudio2エンジンを生成
	HRESULT hr{};
	hr = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	hr = xAudio2->CreateMasteringVoice(&masterVoice);
	// 音声読み込み
	SoundData soundData1 = SoundLoadWave("resources/Alarm01.wav");
	SoundPlayWave(xAudio2.Get(), soundData1);

	//ウィンドウの×ボタンが押されるまでループ
	while (true) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (winApp_->ProcessMessage()) {
			break;
		}
		else {
			// 入力の更新
			input_->Update();

			sprite_->Update();

			// 描画前の処理
			directXEngine_->PreDraw();
			
			// 描画処理
			directXEngine_->Draw();

			// Spriteの描画準備
			SpriteBase::GetInstance()->DrawBase();
			sprite_->Draw();

			// 描画後の処理
			directXEngine_->PostDraw();

			if (input_->TriggerKey(DIK_S)) {
				SoundPlayWave(xAudio2.Get(), soundData1);
			}
		}
	}
	//ImGuiの終了処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	TextureManager::GetInstance()->Finalize();
	SpriteBase::GetInstance()->Finalize();

	xAudio2.Reset();
	SoundUnload(&soundData1);
	winApp_->Finalize();

	return 0;
}