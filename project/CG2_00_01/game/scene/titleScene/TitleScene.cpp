#include "TitleScene.h"
#include "Input.h"

#include "CameraManager.h"
#include "SpriteBase.h"
#include "Object3dBase.h"
#include "ModelManager.h"
#include "ParticleManager.h"

#include "SceneManager.h"

#include "imgui.h"

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

	ModelManager::GetInstance()->LoadModel("resources", "suzanne.obj");
	std::unique_ptr<Object3d> object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize("suzanne.obj");
	object3d_->SetRotation({ 0.0f,3.14f,0.0f });
	obj_.push_back(std::move(object3d_));

	emitter0_ = std::make_unique<ParticleEmitter>("player");
	emitter1_ = std::make_unique<ParticleEmitter>("enemy");
	emitter2_ = std::make_unique<ParticleEmitter>("obj");

	// Particleを作成
	particleManager_->CreateParticleGroup("player", "circle.png", emitter0_.get());
	particleManager_->CreateParticleGroup("enemy", "uvChecker.png", emitter1_.get());
	particleManager_->CreateParticleGroup("obj", "Apple.png", emitter2_.get());

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
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

	if (Input::GetInstance()->PushKey(DIK_F1)) {
		CameraManager::GetInstance()->SetActiveCamera(0);
	}
	if (Input::GetInstance()->PushKey(DIK_F2)) {
		CameraManager::GetInstance()->SetActiveCamera(1);
	}


	for (auto& obj : obj_) {
		obj->Update();
	}
	for (auto& sprite : sprites_) {
		sprite->Update();
	}

	// Particleの更新
	particleManager_->Update();
}

void TitleScene::Draw()
{
	Vector3 axis = { 1.0f,1.0f,1.0f };
	float angle = 0.44f;
	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateAxisAngle(axis, angle);

	ImGui::Begin("MT4");

	if (ImGui::BeginTable("MatrixTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
		for (int j = 0; j < 4; ++j) {
			ImGui::TableNextRow();
			for (int i = 0; i < 4; ++i) {
				ImGui::TableSetColumnIndex(i);
				ImGui::Text("%.3f", rotateMatrix.m[i][j]);
			}
		}
		ImGui::EndTable();
	}

	ImGui::End();

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

	// Particleの描画
	particleManager_->Draw();
}
