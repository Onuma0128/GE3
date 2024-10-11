#include "GamePlayScene.h"

#include "Input.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "PrimitiveDrawer.h"

void GamePlayScene::Initialize()
{
	std::unique_ptr<Sprite> sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("uvChecker.png");
	sprite_->SetPosition({ 64,64 });
	sprite_->SetSize({ 128,128 });
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

	for (float i = 0; i < 10; i++) {

	std::unique_ptr<Line3d> line3d_ = std::make_unique<Line3d>();
	line3d_->Initialize({i,i,i}, {i+1,i + 1 ,i + 1 });
	lines_.push_back(std::move(line3d_));

	}
	/*std::unique_ptr<Line3d> line3d1_ = std::make_unique<Line3d>();
	line3d1_->Initialize({ 2,2,2 }, { 0,1,0 });
	lines_.push_back(std::move(line3d1_));*/

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

void GamePlayScene::Finalize()
{
	xAudio2.Reset();
	SoundUnload(&soundData1);
}

void GamePlayScene::Update()
{
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}

	for (auto& obj : obj_) {
		obj->Update();
	}
	for (auto& sprite : sprites_) {
		sprite->Update();
	}
	for (auto& line : lines_) {
		line->Update();
	}
}

void GamePlayScene::Draw()
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


	PrimitiveDrawer::GetInstance()->DrawBase();
	for (auto& line : lines_) {
		line->Draw();
	}

}
