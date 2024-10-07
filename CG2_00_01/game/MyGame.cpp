#include "MyGame.h"

void MyGame::Initialize()
{
	winApp_ = new WinApp();
	winApp_->Initialize();

	directXEngine_ = new DirectXEngine();
	directXEngine_->Initialize(winApp_);

	input_ = new Input();
	input_->Initialize(winApp_);

	Sprite* sprite_ = new Sprite();
	sprite_->Initialize("uvChecker.png");
	sprite_->SetPosition({ 64,64 });
	sprite_->SetSize({ 128,128 });
	sprite_->SetAnchorPoint({ 0.5f,0.5f });
	sprites_.push_back(sprite_);

	Sprite* sprite1_ = new Sprite();
	sprite1_->Initialize("Apple.png");
	sprite1_->SetPosition({ 640,64 });
	sprite1_->SetSize({ 128,128 });
	sprite1_->SetAnchorPoint({ 0.5f,0.5f });
	sprites_.push_back(sprite1_);

	Object3d* object3d_ = new Object3d();
	object3d_->Initialize();
	object3d_->SetModel("teapot.obj");
	obj_.push_back(object3d_);

	Object3d* object3d1_ = new Object3d();
	object3d1_->Initialize();
	object3d1_->SetModel("terrain.obj");
	obj_.push_back(object3d1_);

	Object3d* object3dTest_ = new Object3d();
	object3dTest_->Initialize();
	object3dTest_->SetModel("plane.gltf");
	object3dTest_->SetRotation({ 0,3.14f,0 });
	obj_.push_back(object3dTest_);

	// オーディオ
	IXAudio2MasteringVoice* masterVoice;
	// XAudio2エンジンを生成
	HRESULT hr{};
	hr = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	hr = xAudio2->CreateMasteringVoice(&masterVoice);
	// 音声読み込み
	soundData1 = SoundLoadWave("resources/Alarm01.wav");
	//SoundPlayWave(xAudio2.Get(), soundData1);
}

void MyGame::Finalize()
{
	//ImGuiの終了処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	xAudio2.Reset();
	SoundUnload(&soundData1);
	winApp_->Finalize();

	for (auto& sprite : sprites_) {
		delete sprite;
		sprite = nullptr;
	}
	for (auto& obj : obj_) {
		delete obj;
		obj = nullptr;
	}

	delete input_;
	delete winApp_;
	delete directXEngine_;
}

void MyGame::Update()
{
	//Windowにメッセージが来てたら最優先で処理させる
	if (winApp_->ProcessMessage()) {
		endRequst_ = true;
	}
	else {
		// 入力の更新
		input_->Update();

		// カメラの更新
		Camera::GetInstance()->Update();

		for (auto& obj : obj_) {
			obj->Update();
		}
		for (auto& sprite : sprites_) {
			sprite->Update();
		}
	}
}

void MyGame::Draw()
{
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
	for (auto& sprite : sprites_) {
		sprite->Draw();
	}

	// 描画後の処理
	directXEngine_->PostDraw();
}
