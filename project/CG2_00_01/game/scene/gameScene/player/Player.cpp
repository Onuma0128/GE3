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

	// モデルの初期化
	transform_ = std::make_unique<WorldTransform>();
	model_ = std::make_unique<Object3d>();
	model_->Initialize("box.obj", transform_.get());

	// 影のモデル
	shadowTransform_ = std::make_unique<WorldTransform>();
	shadowModel_ = std::make_unique<Object3d>();
	shadowModel_->Initialize("player_shadow.obj", shadowTransform_.get());

	// プレイヤーのモデルパーツ
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

	ShadowUpdate();

	LightManager::GetInstance()->SetPointLightPosition(transform_->translation_ + Vector3{ 0,4,0 });
}

void Player::ShadowUpdate()
{
	float translationY = playerAnimation_->GetPlayerModels()->GetBodyTrans()->translation_.y;

	float scale = 
		(global_->GetValue<float>("PlayerShadow", "scalePow") - global_->GetValue<Vector3>("PlayerModelOffset", "head").y) /
		(global_->GetValue<float>("PlayerShadow", "scalePow") - translationY);
	shadowTransform_->scale_ = Vector3{ scale,1.0f,scale };
	Quaternion bodyQ = Quaternion::ExtractYawQuaternion(playerAnimation_->GetPlayerModels()->GetBodyTrans()->rotation_);
	shadowTransform_->rotation_ = transform_->rotation_ * bodyQ;
	shadowTransform_->translation_ = transform_->translation_;
	shadowTransform_->translation_.y = 0.01f;

	float alpha =
		(global_->GetValue<float>("PlayerShadow", "alphaPow") - translationY) /
		(global_->GetValue<float>("PlayerShadow", "alphaPow") - global_->GetValue<Vector3>("PlayerModelOffset", "head").y);
	shadowModel_->SetColor(Vector4{ 0.0f,0.0f,0.0f,alpha });

	shadowModel_->Update();
}

void Player::Draw()
{
	state_->Draw();

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
