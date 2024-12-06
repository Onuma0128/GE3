#include "TitleScene.h"
#include "Input.h"

#include "CameraManager.h"
#include "SpriteBase.h"
#include "Object3dBase.h"
#include "ModelManager.h"
#include "ParticleManager.h"

#include "SceneManager.h"

void TitleScene::Initialize()
{
	// Cameraの初期化
	// ===============
	// Cameraのポインタを初期化
	// CameraManagerに登録
	// ===============
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	CameraManager::GetInstance()->SetCamera(camera_.get());

	camera1_ = std::make_unique<Camera>();
	camera1_->Initialize();
	camera1_->SetRotate(Vector3{ 0.26f,1.57f,0.0f });
	camera1_->SetTranslate(Vector3{ -15.0f,4.0f,0.0f });
	CameraManager::GetInstance()->SetCamera(camera1_.get());

	CameraManager::GetInstance()->SetActiveCamera(0);

	player_ = std::make_unique<Player>();
	player_->Initialize();

	// 地面
	ModelManager::GetInstance()->LoadModel("resources", "plane.obj");
	map_ = std::make_unique<Object3d>();
	map_->Initialize("plane.obj");
	map_->SetScale(Vector3{ 50,50,50 });
	map_->SetRotation(Vector3{ -1.57f,3.14f,0.0f });
	map_->SetPosition(Vector3{ 0.0f,-1.0f,0.0f });

	emitter_ = std::make_unique<ParticleEmitter>("map");
	ParticleManager::GetInstance()->CreateParticleGroup("map", "white1x1.png", emitter_.get());

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
	/*if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}*/

	if (Input::GetInstance()->PushKey(DIK_F1)) {
		CameraManager::GetInstance()->SetActiveCamera(0);
	}
	if (Input::GetInstance()->PushKey(DIK_F2)) {
		CameraManager::GetInstance()->SetActiveCamera(1);
	}

	map_->Update();

	player_->Update();

	ParticleManager::GetInstance()->Update();
}

void TitleScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();

	// 地面の描画
	map_->Draw();

	// プレイヤーの描画
	player_->Draw();

	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();



	// Particleの描画
	ParticleManager::GetInstance()->Draw();
	
}
