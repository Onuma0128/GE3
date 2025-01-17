#include "Player.h"

#include "LightManager.h"
#include "ModelManager.h"
#include "SceneManager.h"

#include "state/MoveState.h"
#include "state/DamageState.h"

void Player::OnCollision(const std::string& name, const Vector3& position)
{
	// 衝突判定は1パターン

	// 1 攻撃を食らった
	if (name == "enemy" && hp_ > 0 && !isDamage_) {
		velocity_ = (transform_->translation_ - position);
		velocity_.y = 0.0f;
		IsDamage();
		audio_->SoundPlayWave("PlayerDamage.wav", 0.5f);
		isShake_ = true;
		isDamage_ = true;
	}
}

Vector3 Player::GetCenterPosition() const
{
	return Vector3{}.Transform(transform_->matWorld_);
}

std::string Player::GetColliderName() const
{
	return "player";
}

float Player::GetRadius() const
{
	return global_->GetValue<float>("Collider", "playerRadius");
}

Player::~Player()
{
	state_->Finalize();
}

void Player::Init()
{
	GlobalInit();

	audio_ = std::make_unique<Audio>();

	// モデルの初期化
	transform_ = std::make_unique<WorldTransform>();
	transform_->translation_ = global_->GetValue<Vector3>("Player", "position");
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

	state_ = std::move(std::make_unique<MoveState>(this, playerAnimation_.get()));
	state_->Initialize();

	// プレイヤーのUI
	playerUI_ = std::make_unique<PlayerUI>();
	playerUI_->Init();
	playerUI_->SetPlayer(this);

	// プレイヤー周りのパーティクル
	playerParticle_ = std::make_unique<PlayerParticle>();
	playerParticle_->Init();
	playerParticle_->SetPlayer(this);

	// プレイヤーのエフェクト
	playerEffect_ = std::make_unique<PlayerEffect>();
	playerEffect_->Init();
	playerEffect_->SetPlayer(this);

	fade_ = std::make_unique<FadeScene>();
	fade_->Init(0.0f);

}

void Player::Update()
{
	playerAnimation_->Debug_ImGui();

	state_->Update();
	DamageUpdate();

	model_->Update();

	ShadowUpdate();

	playerUI_->Update();

	playerParticle_->Update();

	playerEffect_->Update();

	SetLight();

	if (hp_ <= 0) {
		fade_->FadeIn("GameOver", Vector3{ 0.0f,0.0f,0.0f }, 120.0f);
	}
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

	playerParticle_->Draw();
}

void Player::DrawSprite()
{
	playerUI_->Draw();

	fade_->Draw();
}

void Player::DrawTrail()
{
	playerEffect_->DrawTrail();
}

void Player::IsDamage()
{
	if (velocity_.x != 0.0f || velocity_.z != 0.0f) {
		velocity_.Normalize();
	}
	else {
		velocity_ = -Vector3::ExprUnitZ;
	}
	playerAnimation_->Reset();
	playerAnimation_->GetPlayerModels()->ModelOffset();
	isAttack_ = false;
	playerParticle_->GetSwordEmitter()->SetIsCreate(false);
	ChengeState(std::make_unique<DamageState>(this, playerAnimation_.get()));
	if (hp_ > 0) {
		playerUI_->DeleteHP();
	}
	--hp_;
}

void Player::DamageUpdate()
{
	if (isDamage_) {
		damageFrame_ += 1.0f / 150.0f;
	}
	if (damageFrame_ >= 1.0f) {
		isDamage_ = false;
		damageFrame_ = 0.0f;
	}
}

void Player::SetLight()
{
	if (static_cast<int>(damageFrame_ * 150.0f) % 10 == 0) {
		LightManager::GetInstance()->GetPointLightData()->intensity = 1.0f;
	}
	else {
		LightManager::GetInstance()->GetPointLightData()->intensity = 0.1f;
	}

	LightManager::GetInstance()->SetPointLightPosition(transform_->translation_ + Vector3{ 0,4,0 });
}

void Player::ChengeState(std::unique_ptr<BaseState> newState)
{
	state_->Finalize();
	state_ = std::move(newState);
	state_->Initialize();
}

void Player::GlobalInit()
{
	global_->AddValue<Vector3>("Player", "position", Vector3{});
	global_->AddValue<float>("Player", "moveSpeed", 0.1f);
	global_->AddValue<float>("Player", "velocityY", 1.0f);
	global_->AddValue<float>("Player", "accelerationY", 0.1f);
	global_->AddValue<float>("Player", "attackVelocityY", 0.05f);
	global_->AddValue<float>("Player", "slerpSpeed", 0.1f);
	global_->AddValue<float>("Player", "dustAccelerationY", 1.0f);
	global_->AddValue<float>("Player", "dustAcceleration", -1.0f);
	global_->AddValue<float>("Player", "knockbackFrame", 10.0f);
	global_->AddValue<float>("Player", "dashPow", 3.0f);

	global_->AddValue<Vector3>("PlayerSwordParticle", "position", Vector3{});
	global_->AddValue<Vector3>("PlayerSwordParticle", "acceleration", Vector3{});
	global_->AddValue<Vector3>("PlayerSwordParticle", "acceleration2", Vector3{});
	global_->AddValue<Vector3>("PlayerSwordParticle", "acceleration3", Vector3{});
	global_->AddValue<Vector3>("PlayerSwordParticle", "acceleration4", Vector3{});

	global_->AddValue<float>("PlayerShadow", "scalePow", 25.0f);
	global_->AddValue<float>("PlayerShadow", "alphaPow", 10.0f);


	global_->AddValue<float>("Collider", "playerRadius", 1.0f);
}
