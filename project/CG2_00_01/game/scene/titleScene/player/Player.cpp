#include "Player.h"

#include "ModelManager.h"

#include "titleScene/player/state/Move.h"

void Player::Initialize()
{
	GlobalInitialize();

	// プレイヤーの初期化
	ModelManager::GetInstance()->LoadModel("resources", "suzanne.obj");

	model_ = std::make_unique<Object3d>();
	model_->Initialize("suzanne.obj");
	model_->SetRotation(Vector3{ 0.0f,3.14f,0.0f });

	// パーティクルの初期化
	emitter_ = std::make_unique<ParticleEmitter>("player");
	particleManager_->CreateParticleGroup("player", "circle.png", emitter_.get());
	
	// 炎のパーティクル
	emitterbackFlame_ = std::make_unique<ParticleEmitter>("backFlame");
	emitterFlame_ = std::make_unique<ParticleEmitter>("flame");
	particleManager_->CreateParticleGroup("backFlame", "circle.png", emitterbackFlame_.get());
	particleManager_->CreateParticleGroup("flame", "circle.png", emitterFlame_.get());

	state_ = std::make_unique<Move>(this);
	state_->Initialize();
}

void Player::GlobalInitialize()
{
	
}

void Player::Update()
{	
	// プレイヤーの行動
	state_->Update();

	model_->Update();

	emitterbackFlame_->SetPosition(emitterFlame_->GetPosition());

	emitter_->SetPosition(model_->GetPosition());
}

void Player::Draw()
{
	state_->Draw();

	model_->Draw();
}

void Player::changeState(std::unique_ptr<PlayerBase> newState)
{
	state_ = std::move(newState);
	state_->Initialize();
}
