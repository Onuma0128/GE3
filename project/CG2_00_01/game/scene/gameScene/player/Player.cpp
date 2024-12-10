#include "Player.h"

#include <numbers>

#include "LightManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"

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

	state_ = State::Move;

	// パーティクルの初期化
	emitter_ = std::make_unique<ParticleEmitter>("playerDust");
	ParticleManager::GetInstance()->CreateParticleGroup("playerDust", "white1x1.png", emitter_.get());
}

void Player::Update()
{
	emitter_->SetIsCreate(false);

	switch (state_)
	{
	case Player::State::Move:
		Move();
		break;
	case Player::State::Jump:
		Jump();
		break;
	case Player::State::Attack:
		Attack();
		break;
	default:
		break;
	}

	emitter_->SetPosition(model_->GetPosition());
	Vector3 acceleration = velocity_ * -1.0f;
	acceleration.y = 1.0f;
	emitter_->SetAcceleration(acceleration);

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
	model_->Draw();

	shadowModel_->Draw();

	for (auto& attackParticle : attackParticles_) {
		attackParticle->Draw();
	}
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
}

void Player::Move()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		state_ = State::Jump;
		velocity_.y = global_->GetValue<float>("Player", "velocityY");
		acceleration_.y = global_->GetValue<float>("Player", "accelerationY");
		return;
	}

	Vector3 velocity = {};

	if (input_->PushKey(DIK_W)) {
		velocity.z = 1.0f;
	}
	if (input_->PushKey(DIK_S)) {
		velocity.z = -1.0f;
	}
	if (input_->PushKey(DIK_A)) {
		velocity.x = -1.0f;
	}
	if (input_->PushKey(DIK_D)) {
		velocity.x = 1.0f;
	}
	if (velocity.x != 0.0f || velocity.z != 0.0f) {
		emitter_->SetIsCreate(true);
		velocity.Normalize();
		velocity_ = velocity;
	}

	// 回転の処理
	float newRotateY = std::atan2(velocity_.x, velocity_.z);
	float nowRotateY = model_->GetRotation().y;
	float rotate = LerpShortAngle(nowRotateY, newRotateY, 0.2f);
	model_->SetRotation(Vector3{ 0.0f,rotate ,0.0f });

	// 移動の処理
	Vector3 translate = model_->GetPosition();
	translate += velocity * 0.03f;
	model_->SetPosition(translate);

	// 影の処理
	translate.y = 0.01f;
	shadowModel_->SetRotation(model_->GetRotation());
	shadowModel_->SetPosition(translate);
}

void Player::Jump()
{
	Vector3 translate = model_->GetPosition();
	velocity_.y -= acceleration_.y;
	translate.y += velocity_.y;
	model_->SetPosition(translate);

	if (model_->GetPosition().y > 2.5f) {
		if (input_->TriggerKey(DIK_SPACE)) {
			state_ = State::Attack;
			velocity_.y = global_->GetValue<float>("Player", "attackVelocityY") * -1.0f;
		}
	}

	if (model_->GetPosition().y < 0.5f) {
		translate.y = 0.5f;
		model_->SetPosition(translate);
		velocity_.y = 0.0f;
		acceleration_.y = 0.0f;
		state_ = State::Move;
	}
	// 影のサイズ、透明度の処理
	float scale = (50.0f - translate.y) / (50.0f - 0.5f);
	shadowModel_->SetScale(Vector3{ scale,0.01f,scale });
	shadowModel_->SetColor(Vector4{ 0.0f,0.0f,0.0f,scale });
}

void Player::Attack()
{
	if (attackAnimaFrame_ < 1.0f) {
		attackAnimaFrame_ += 1.0f / 20.0f;
		if (attackAnimaFrame_ >= 1.0f) {
			attackAnimaFrame_ = 1.0f;
		}
		float rotateX = 2.0f * static_cast<float>(std::numbers::pi) * attackAnimaFrame_;
		float rotateY = std::atan2(velocity_.x, velocity_.z);
		model_->SetRotation(Vector3{ rotateX ,rotateY,0.0f });
	}
	else {
		attackAnimaFrame_ = 1.0f;

		Vector3 translate = model_->GetPosition();
		translate.y += velocity_.y;
		model_->SetPosition(translate);

		if (model_->GetPosition().y < 0.5f) {
			isShake_ = true;
			attackAnimaFrame_ = 0.0f;
			translate.y = 0.5f;
			model_->SetPosition(translate);
			velocity_.y = 0.0f;
			acceleration_.y = 0.0f;
			state_ = State::Move;
			int count = global_->GetValue<int>("PlayerAttackParticle", "count");
			for (int i = 0; i < count; ++i) {
				std::unique_ptr<AttackParticle> particle = std::make_unique<AttackParticle>();
				particle->Init("attackParticle.obj", model_->GetPosition());
				attackParticles_.push_back(std::move(particle));
			}
		}
	}
}

float Player::LerpShortAngle(float a, float b, float t)
{
	float diff = b - a;

	// 角度を [-2PI, +2PI] に補正する
	while (diff > 2 * std::numbers::pi_v<float>) {
		diff -= 2 * std::numbers::pi_v<float>;
	}
	while (diff < -2 * std::numbers::pi) {
		diff += 2 * std::numbers::pi_v<float>;
	}

	// 角度を [-PI, +PI] に補正する
	if (diff > std::numbers::pi) {
		diff -= 2 * std::numbers::pi_v<float>;
	}
	else if (diff < -std::numbers::pi_v<float>) {
		diff += 2 * std::numbers::pi_v<float>;
	}

	// 線形補間を行う
	return a + t * diff;
}
