#include "Player.h"

#include "LightManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"

#include "state/MoveState.h"

Player::~Player()
{
	state_->Finalize();
}

void Player::Init()
{
	GlobalInit();

	ModelManager::GetInstance()->LoadModel("resources", "box.obj");
	ModelManager::GetInstance()->LoadModel("resources", "attackParticle.obj");

	// モデルの初期化
	model_ = std::make_unique<Object3d>();
	model_->Initialize("box.obj");
	model_->SetTexture("resources", "uvChecker.png");
	model_->SetPosition(Vector3{ 0.0f,0.5f,0.0f });

	shadowModel_ = std::make_unique<Object3d>();
	shadowModel_->Initialize("box.obj");
	shadowModel_->SetScale(Vector3{ 1.0f,0.01f,1.0f });
	shadowModel_->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });

	// パーティクルの初期化
	moveParticleEmitter_ = std::make_unique<ParticleEmitter>("playerDust");
	ParticleManager::GetInstance()->CreateParticleGroup("playerDust", "white1x1.png", moveParticleEmitter_.get());

	ChengeState(std::make_unique<MoveState>(this));
}

void Player::Update()
{
	state_->Update();

	model_->Update();
	shadowModel_->Update();

	for (auto& attackParticle : attackParticles_) {
		attackParticle->Update();
	}

	attackParticles_.remove_if([](const std::unique_ptr<AttackParticle>& particle) {
		return !particle->GetIsActive();
	});

	LightManager::GetInstance()->SetPointLightPosition(model_->GetPosition() + Vector3{ 0,2,0 });
}

void Player::Draw()
{
	state_->Draw();

	model_->Draw();

	shadowModel_->Draw();

	for (auto& attackParticle : attackParticles_) {
		attackParticle->Draw();
	}
}

void Player::ChengeState(std::unique_ptr<BaseState> newState)
{
	state_ = std::move(newState);
	state_->Initialize();
}

void Player::GlobalInit()
{
	global_->AddValue<float>("Player", "velocityY", 1.0f);
	global_->AddValue<float>("Player", "accelerationY", 0.1f);
	global_->AddValue<float>("Player", "attackVelocityY", 0.05f);

	global_->AddValue<int>("PlayerAttackParticle", "count", 20);
	global_->AddValue<float>("PlayerAttackParticle", "scale", 1.0f);
	global_->AddValue<float>("PlayerAttackParticle", "velocityPow", 0.1f);
	global_->AddValue<Vector3>("PlayerAttackParticle", "offset", Vector3{ 0,0,0 });
	global_->AddValue<float>("PlayerAttackParticle", "alphaSubtrac", 0.01f);
}
