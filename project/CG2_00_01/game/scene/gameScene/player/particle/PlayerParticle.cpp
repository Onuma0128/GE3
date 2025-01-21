#include "PlayerParticle.h"

#include <random>

#include "ParticleManager.h"
#include "imgui.h"

#include "gameScene/player/Player.h"

void PlayerParticle::Init()
{
	GlobalInit();

	// 移動時パーティクルの初期化
	moveParticleEmitter_ = std::make_unique<ParticleEmitter>("playerDust");
	ParticleManager::GetInstance()->CreateParticleGroup("playerDust", "white1x1.png", moveParticleEmitter_.get());
	
	for (int i = 0; i < 20; ++i) {
		Combo3Particle particle{};
		// Transformの初期化
		particle.transform_ = std::make_unique<WorldTransform>();
		// モデルを初期化
		particle.model_ = std::make_unique<Object3d>();
		particle.model_->Initialize("box.obj", particle.transform_.get());
		// 速度の初期化
		particle.velocity_ = {};
		particle.alpha_ = 0.0f;
		combo3Particles_.push_back(std::move(particle));
	}
	// 地面のひび割れ
	transform_ = std::make_unique<WorldTransform>();
	model_ = std::make_unique<Object3d>();
	model_->Initialize("crack.obj", transform_.get());
	model_->SetTexture("resources","crack.png");
	model_->SetColor(Vector4{ 1.0f,1.0f,1.0f,0.0f });
	alpha_ = 0.0f;

}

void PlayerParticle::GlobalInit()
{
	global_->AddValue<int>("PlayerAttackParitcle", "particleCount", 1);
	global_->AddValue<int>("PlayerAttackParitcle", "randomSize", 7);
	global_->AddValue<float>("PlayerAttackParitcle", "velocityPow", 0.1f);
	global_->AddValue<float>("PlayerAttackParitcle", "accelerationY", 0.1f);
	global_->AddValue<float>("PlayerAttackParitcle", "offsetVelocityY", 1.0f);
	global_->AddValue<float>("PlayerAttackParitcle", "particleSize", 1.0f);
	global_->AddValue<float>("PlayerAttackParitcle", "alpha", 0.1f);

	global_->AddValue<Vector3>("PlayerAttackParitcle", "offsetPosition", Vector3{});
	global_->AddValue<float>("PlayerAttackParitcle", "offsetSize", 1.0f);
}

void PlayerParticle::Update()
{
	for (auto& particle : combo3Particles_) {
		if (particle.transform_->translation_.y >= 0.0f) {
			Quaternion rotateX = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, particle.velocity_.x);
			Quaternion rotateZ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitZ, particle.velocity_.z);
			particle.transform_->rotation_.Slerp((rotateX * rotateZ), 0.1f);
			particle.velocity_.y -= global_->GetValue<float>("PlayerAttackParitcle", "accelerationY");
			particle.transform_->translation_ += particle.velocity_ * global_->GetValue<float>("PlayerAttackParitcle", "velocityPow");
			particle.model_->Update();
		}
		else if (particle.alpha_ > 0.0f) {
			particle.alpha_ -= global_->GetValue<float>("PlayerAttackParitcle", "alpha");
			particle.model_->SetColor(Vector4{ 1.0f,1.0f,1.0f,particle.alpha_ });
			particle.model_->Update();
		}
		else {
			particle.alpha_ = 0.0f;
			particle.model_->SetColor(Vector4{ 1.0f,1.0f,1.0f,particle.alpha_ });
		}
	}
	if (alpha_ > 0.0f) {
		alpha_ -= global_->GetValue<float>("PlayerAttackParitcle", "alpha");
		model_->SetColor(Vector4{ 1.0f,1.0f,1.0f,alpha_ });
	}
	else {
		alpha_ = 0.0f;
		model_->SetColor(Vector4{ 1.0f,1.0f,1.0f,alpha_ });
	}

	model_->Update();
}

void PlayerParticle::Draw()
{
	for (auto& particle : combo3Particles_) {
		if (particle.alpha_ > 0.0f) {
			particle.model_->Draw();
		}
	}
	
	model_->Draw();
}

void PlayerParticle::CreateParticle(const Vector3& position)
{
	int num = global_->GetValue<int>("PlayerAttackParitcle", "particleCount");
	int count = 0;

	for (auto& particle : combo3Particles_) {
		if (particle.alpha_ == 0.0f) {
			// Transformの初期化
			float scale = global_->GetValue<float>("PlayerAttackParitcle", "particleSize") + static_cast<float>(rand() % 3 + 1) * 0.1f;
			particle.transform_->scale_ = { scale ,scale ,scale };
			particle.transform_->translation_ = position;
			// 速度の初期化
			int size = global_->GetValue<int>("PlayerAttackParitcle", "randomSize");
			particle.velocity_ = {
				static_cast<float>(rand() % size - (size - 1) / 2),
				global_->GetValue<float>("PlayerAttackParitcle", "offsetVelocityY"),
				static_cast<float>(rand() % size - (size - 1) / 2) };
			particle.velocity_.Normalize();
			particle.alpha_ = 1.0f;

			// カウントを加算
			++count;
			if (num == count) {
				break;
			}
		}
	}

	alpha_ = 1.0f;
	float offsetScale = global_->GetValue<float>("PlayerAttackParitcle", "offsetSize");
	transform_->scale_ = { offsetScale ,offsetScale ,offsetScale };
	transform_->rotation_ = player_->GetTransform()->rotation_;
	transform_->translation_ = position + global_->GetValue<Vector3>("PlayerAttackParitcle", "offsetPosition");
}
