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
	transform_ = std::make_unique<WorldTransform>();
	model_ = std::make_unique<Object3d>();
	model_->Initialize("box.obj", transform_.get());
	model_->SetTexture("resources", "uvChecker.png");
	transform_->translation_ = Vector3{ 0.0f,0.5f,0.0f };

	shadowTransform_ = std::make_unique<WorldTransform>();
	shadowModel_ = std::make_unique<Object3d>();
	shadowModel_->Initialize("box.obj", shadowTransform_.get());
	shadowModel_->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });
	shadowTransform_->scale_ = Vector3{ 1.0f,0.01f,1.0f };

	playerAnimation_ = std::make_unique<PlayerAnimation>();
	playerAnimation_->SetPlayer(this);
	playerAnimation_->Init();

	// パーティクルの初期化
	moveParticleEmitter_ = std::make_unique<ParticleEmitter>("playerDust");
	ParticleManager::GetInstance()->CreateParticleGroup("playerDust", "white1x1.png", moveParticleEmitter_.get());

	ChengeState(std::make_unique<MoveState>(this, playerAnimation_.get()));
}

void Player::Update()
{
	state_->Update();

	model_->Update();

	shadowModel_->Update();

	LightManager::GetInstance()->SetPointLightPosition(transform_->translation_ + Vector3{ 0,4,0 });
}

void Player::ShadowUpdate()
{
	float scale = 
		(global_->GetValue<float>("PlayerShadow", "scalePow") - 0.5f) / 
		(global_->GetValue<float>("PlayerShadow", "scalePow") - transform_->translation_.y);
	shadowTransform_->scale_ = Vector3{ scale,0.01f,scale };
	float alpha =
		(global_->GetValue<float>("PlayerShadow", "alphaPow") - transform_->translation_.y) / 
		(global_->GetValue<float>("PlayerShadow", "alphaPow") - 0.5f);
	shadowModel_->SetColor(Vector4{ 0.0f,0.0f,0.0f,alpha });
}

void Player::Draw()
{
	state_->Draw();

	//model_->Draw();

	shadowModel_->Draw();
}

void Player::ChengeState(std::unique_ptr<BaseState> newState)
{
	state_ = std::move(newState);
	state_->Initialize();
}

void Player::GlobalInit()
{
	global_->AddValue<float>("Player", "moveSpeed", 0.1f);
	global_->AddValue<float>("Player", "velocityY", 1.0f);
	global_->AddValue<float>("Player", "accelerationY", 0.1f);
	global_->AddValue<float>("Player", "attackVelocityY", 0.05f);
	global_->AddValue<float>("Player", "slerpSpeed", 0.1f);

	global_->AddValue<float>("PlayerAttack", "attackAnimaFrame", 20.0f);

	global_->AddValue<int>("PlayerAttackParticle", "count", 20);
	global_->AddValue<float>("PlayerAttackParticle", "scale", 1.0f);
	global_->AddValue<float>("PlayerAttackParticle", "velocityPow", 0.1f);
	global_->AddValue<Vector3>("PlayerAttackParticle", "offset", Vector3{ 0,0,0 });
	global_->AddValue<float>("PlayerAttackParticle", "alphaSubtrac", 0.01f);

	global_->AddValue<float>("PlayerShadow", "scalePow", 25.0f);
	global_->AddValue<float>("PlayerShadow", "alphaPow", 10.0f);
}
