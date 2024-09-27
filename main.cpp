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
#include "Model.h"
#include "ModelBase.h"
#include "TextureManager.h"
#include "LightManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	std::unique_ptr<WinApp>winApp_ = std::make_unique<WinApp>();
	winApp_->Initialize();

	std::unique_ptr<DirectXEngine> directXEngine_ = std::make_unique<DirectXEngine>();
	directXEngine_->Initialize(winApp_.get());

	std::unique_ptr<Input> input_ = std::make_unique<Input>();
	input_->Initialize(winApp_.get());

	/*==================== ライト準備用 ====================*/

	LightManager::GetInstance()->Initialize(directXEngine_.get());

	/*==================== モデル描画準備用 ====================*/

	Object3dBase::GetInstance()->Initialize(directXEngine_.get());
	ModelBase::GetInstance()->Initialize(directXEngine_.get());

	/*==================== スプライト描画準備用 ====================*/
	
	SpriteBase::GetInstance()->Initialize(directXEngine_.get());

	/*==================== テクスチャ読み込み ====================*/

	TextureManager::GetInstance()->Initialize(directXEngine_.get());

	std::unique_ptr<Sprite> sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("uvChecker.png");
	sprite_->SetPosition({ 64,64 });
	sprite_->SetSize({ 128,128 });
	sprite_->SetAnchorPoint({ 0.5f,0.5f });

	std::unique_ptr<Object3d> object3d_ = std::make_unique<Object3d>();
	std::unique_ptr<Model> model_ = std::make_unique<Model>();
	object3d_->Initialize();
	model_->Initialize();
	object3d_->SetModel(model_.get());

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

			object3d_->Update();
			sprite_->Update();

			// 描画前の処理
			directXEngine_->PreDraw();
			
			// 描画処理
			directXEngine_->Draw();

			// ライトの更新
			LightManager::GetInstance()->Update();

			// Modelの描画準備
			Object3dBase::GetInstance()->DrawBase();
			object3d_->Draw();

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
	Object3dBase::GetInstance()->Finalize();
	ModelBase::GetInstance()->Finalize();
	LightManager::GetInstance()->Finalize();

	xAudio2.Reset();
	SoundUnload(&soundData1);
	winApp_->Finalize();

	return 0;
}