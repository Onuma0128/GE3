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
#include "ModelManager.h"
#include "TextureManager.h"
#include "LightManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WinApp* winApp_ = new WinApp();
	winApp_->Initialize();

	DirectXEngine* directXEngine_ = new DirectXEngine();
	directXEngine_->Initialize(winApp_);

	Input* input_ = new Input();
	input_->Initialize(winApp_);

	/*==================== ライト準備用 ====================*/

	LightManager::GetInstance()->Initialize(directXEngine_);

	/*==================== モデル描画準備用 ====================*/

	Object3dBase::GetInstance()->Initialize(directXEngine_);

	/*==================== スプライト描画準備用 ====================*/
	
	SpriteBase::GetInstance()->Initialize(directXEngine_);

	/*==================== テクスチャ読み込み ====================*/

	TextureManager::GetInstance()->Initialize(directXEngine_);

	/*==================== モデル読み込み ====================*/

	ModelManager::GetInstance()->Initialize(directXEngine_);

	Sprite* sprite_ = new Sprite();
	sprite_->Initialize("uvChecker.png");
	sprite_->SetPosition({ 64,64 });
	sprite_->SetSize({ 128,128 });
	sprite_->SetAnchorPoint({ 0.5f,0.5f });

	std::vector<Object3d*> obj_;

	Object3d* object3d_ = new Object3d();
	object3d_->Initialize();
	object3d_->SetModel("terrain.obj");
	obj_.push_back(object3d_);

	Object3d* object3dTest_ = new Object3d();
	object3dTest_->Initialize();
	object3dTest_->SetModel("teapot.obj");
	obj_.push_back(object3dTest_);

	/*Object3d* object3dTest_ = new Object3d();
	object3dTest_->Initialize();
	object3dTest_->SetModel("terrain.obj");*/

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

			
			for (auto& obj : obj_) {
				obj->Update();
			}

			sprite_->Update();

			// 描画前の処理
			directXEngine_->PreDraw();
			
			// 描画処理
			directXEngine_->Draw();

			// ライトの更新
			LightManager::GetInstance()->Update();

			// Modelの描画準備
			Object3dBase::GetInstance()->DrawBase();
			for (auto& obj : obj_) {
				obj->Draw();
			}

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
	LightManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();

	xAudio2.Reset();
	SoundUnload(&soundData1);
	winApp_->Finalize();

	delete input_;

	delete directXEngine_;

	return 0;
}