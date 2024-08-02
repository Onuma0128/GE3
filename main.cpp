#include <dxgidebug.h>
#pragma comment(lib,"dxguid.lib")
#include "wrl.h"
#include "Input.h"
#include "DirectXEngine.h"
#include "D3DResourceLeakChecker.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakChecker leakChecker;

	WinApp* winApp_ = new WinApp();
	winApp_->Initialize();

	DirectXEngine* directXEngine_ = new DirectXEngine();
	directXEngine_->Initialize(winApp_);

	Input* input_ = new Input();
	input_->Initialize(winApp_);

	//// オーディオ
	//ComPtr<IXAudio2> xAudio2;
	//IXAudio2MasteringVoice* masterVoice;
	//// XAudio2エンジンを生成
	//hr = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//hr = xAudio2->CreateMasteringVoice(&masterVoice);
	//// 音声読み込み
	//SoundData soundData1 = SoundLoadWave("resources/fanfare.wav");
	//SoundPlayWave(xAudio2.Get(), soundData1);

	//ウィンドウの×ボタンが押されるまでループ
	while (true) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (winApp_->ProcessMessage()) {
			break;
		}
		else {
			// 入力の更新
			input_->Update();

			// 描画前の処理
			directXEngine_->PreDraw();

			// 描画処理
			directXEngine_->Draw();

			// 描画後の処理
			directXEngine_->PostDraw();

			if (input_->TriggerKey(DIK_S)) {
				OutputDebugStringA("Hit S\n");
			}
		}
	}
	//ImGuiの終了処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	/*xAudio2.Reset();
	SoundUnload(&soundData1);*/
	delete input_;

	delete directXEngine_;
	winApp_->Finalize();
	delete winApp_;

	return 0;
}