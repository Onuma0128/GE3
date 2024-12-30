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

	shadowTransform_ = std::make_unique<WorldTransform>();
	shadowModel_ = std::make_unique<Object3d>();
	shadowModel_->Initialize("box.obj", shadowTransform_.get());
	shadowModel_->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });
	shadowTransform_->scale_ = Vector3{ 1.0f,0.01f,1.0f };

	playerAnimation_ = std::make_unique<PlayerAnimation>();
	playerAnimation_->SetPlayer(this);
	playerAnimation_->Init();

	// 移動時パーティクルの初期化
	moveParticleEmitter_ = std::make_unique<ParticleEmitter>("playerDust");
	ParticleManager::GetInstance()->CreateParticleGroup("playerDust", "white1x1.png", moveParticleEmitter_.get());

	// 攻撃時パーティクルの初期化
	swordParticleEmitter_ = std::make_unique<ParticleEmitter>("sword");
	ParticleManager::GetInstance()->CreateParticleGroup("sword", "circle.png", swordParticleEmitter_.get());
	swordParticleEmitter_->SetIsCreate(false);


	state_ = std::move(std::make_unique<MoveState>(this, playerAnimation_.get()));
	state_->Initialize();
}

void Player::Update()
{
	playerAnimation_->Debug_ImGui();

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
	state_->Finalize();
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

	global_->AddValue<Vector3>("PlayerSwordParticle", "position", Vector3{});
	global_->AddValue<Vector3>("PlayerSwordParticle", "acceleration", Vector3{});
	global_->AddValue<Vector3>("PlayerSwordParticle", "acceleration2", Vector3{});
	global_->AddValue<Vector3>("PlayerSwordParticle", "acceleration3", Vector3{});

	global_->AddValue<float>("PlayerShadow", "scalePow", 25.0f);
	global_->AddValue<float>("PlayerShadow", "alphaPow", 10.0f);
}
