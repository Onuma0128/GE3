#include "GamePlayScene.h"

#include "Input.h"
#include "SceneManager.h"
#include "PrimitiveDrawer.h"
#include "ModelManager.h"
#include "ParticleManager.h"

#include "titleScene/TitleScene.h"

void GamePlayScene::Initialize()
{
	camera_ = std::make_unique<GameCamera>();
	camera_->Init();

	ModelManager::GetInstance()->LoadModel("resources", "ground.obj");

	std::unique_ptr<Object3d> object3d = std::make_unique<Object3d>();
	object3d->Initialize("ground.obj");
	obj_.push_back(std::move(object3d));

	player_ = std::make_unique<Player>();
	player_->Init();

	camera_->SetPlayer(player_.get());

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

	camera_->Update();

	player_->Update();

	// 全パーティクルの更新
	ParticleManager::GetInstance()->Update();
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


	// 全パーティクルの描画
	ParticleManager::GetInstance()->Draw();

}
