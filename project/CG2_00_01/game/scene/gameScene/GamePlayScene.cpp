#include "GamePlayScene.h"

#include "CameraManager.h"
#include "Input.h"
#include "SceneManager.h"
#include "PrimitiveDrawer.h"
#include "ModelManager.h"

#include "titleScene/TitleScene.h"

void GamePlayScene::Initialize()
{
	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetRotate(Vector3{ 0.33f,0.0f,0.0f });
	camera_->SetTranslate(Vector3{ 0.0f,7.0f,-22.0f });
	CameraManager::GetInstance()->SetCamera(camera_.get());
	camera_->Update();

	ModelManager::GetInstance()->LoadModel("resources", "ground.obj");

	std::unique_ptr<Object3d> object3d = std::make_unique<Object3d>();
	object3d->Initialize("ground.obj");
	obj_.push_back(std::move(object3d));

	player_ = std::make_unique<Player>();
	player_->Init();

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
	for (auto& obj : obj_) {
		obj->Update();
	}

	player_->Update();
}

void GamePlayScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();
	for (auto& obj : obj_) {
		obj->Draw();
	}

	player_->Draw();

	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();



	PrimitiveDrawer::GetInstance()->DrawBase();

}
