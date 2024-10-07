#include "TitleScene.h"

void TitleScene::Initialize()
{
	std::unique_ptr<Sprite> sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("uvChecker.png");
	sprite_->SetPosition({ 640,320 });
	sprite_->SetSize({ 400,400 });
	sprite_->SetAnchorPoint({ 0.5f,0.5f });
	sprites_.push_back(std::move(sprite_));

	std::unique_ptr<Sprite> sprite1_ = std::make_unique<Sprite>();
	sprite1_->Initialize("Apple.png");
	sprite1_->SetPosition({ 640,64 });
	sprite1_->SetSize({ 128,128 });
	sprite1_->SetAnchorPoint({ 0.5f,0.5f });
	sprites_.push_back(std::move(sprite1_));

	std::unique_ptr<Object3d> object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel("teapot.obj");
	obj_.push_back(std::move(object3d_));

	std::unique_ptr<Object3d> object3d1_ = std::make_unique<Object3d>();
	object3d1_->Initialize();
	object3d1_->SetModel("terrain.obj");
	obj_.push_back(std::move(object3d1_));

	std::unique_ptr<Object3d> object3dTest_ = std::make_unique<Object3d>();
	object3dTest_->Initialize();
	object3dTest_->SetModel("plane.gltf");
	object3dTest_->SetRotation({ 0,3.14f,0 });
	obj_.push_back(std::move(object3dTest_));

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

void TitleScene::Finalize()
{
	xAudio2.Reset();
	SoundUnload(&soundData1);
}

void TitleScene::Update()
{
	for (auto& obj : obj_) {
		obj->Update();
	}
	for (auto& sprite : sprites_) {
		sprite->Update();
	}
}

void TitleScene::Draw()
{
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
}
