#include "PlayerParticle.h"

#include <random>

#include "ParticleManager.h"
#include "imgui.h"

void PlayerParticle::Init()
{
	GlobalInit();

	// 移動時パーティクルの初期化
	moveParticleEmitter_ = std::make_unique<ParticleEmitter>("playerDust");
	ParticleManager::GetInstance()->CreateParticleGroup("playerDust", "white1x1.png", moveParticleEmitter_.get());

	// 攻撃時パーティクルの初期化
	swordParticleEmitter_ = std::make_unique<ParticleEmitter>("sword");
	ParticleManager::GetInstance()->CreateParticleGroup("sword", "circle.png", swordParticleEmitter_.get());
	swordParticleEmitter_->SetIsCreate(false);
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
}

void PlayerParticle::Update()
{
	/*ImGui::Begin("Combo3Particle");
	if (ImGui::Button("CreateParticle")) {
		CreateParticle();
	}
	ImGui::End();*/

	for (auto& particle : combo3Particles_) {
		if (particle.transform_->translation_.y >= 0.0f) {
			Quaternion rotateX = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, particle.velocity_.x);
			Quaternion rotateZ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitZ, particle.velocity_.z);
			particle.transform_->rotation_.Slerp((rotateX * rotateZ), 0.1f);
			particle.velocity_.y -= global_->GetValue<float>("PlayerAttackParitcle", "accelerationY");
			particle.transform_->translation_ += particle.velocity_ * global_->GetValue<float>("PlayerAttackParitcle", "velocityPow");
			particle.model_->Update();
		}
		else {
			particle.alpha_ -= global_->GetValue<float>("PlayerAttackParitcle", "alpha");
			particle.model_->SetColor(Vector4{ 1.0f,1.0f,1.0f,particle.alpha_ });
			particle.model_->Update();
		}
	}
	for (auto it = combo3Particles_.begin(); it != combo3Particles_.end();) {
		if ((it)->alpha_ <= 0.0f) {
			it = combo3Particles_.erase(it);
		}
		else {
			++it;
		}
	}
}

void PlayerParticle::Draw()
{
	for (auto& particle : combo3Particles_) {
		particle.model_->Draw();
	}
}

void PlayerParticle::CreateParticle(const Vector3& position)
{
	for (int i = 0; i < global_->GetValue<int>("PlayerAttackParitcle", "particleCount"); ++i) {
		Combo3Particle particle{};
		particle.transform_ = std::make_unique<WorldTransform>();
		float scale = global_->GetValue<float>("PlayerAttackParitcle", "particleSize");
		particle.transform_->scale_ = { scale ,scale ,scale };
		particle.transform_->translation_ = position;
		particle.model_ = std::make_unique<Object3d>();
		particle.model_->Initialize("box.obj", particle.transform_.get());
		int size = global_->GetValue<int>("PlayerAttackParitcle", "randomSize");
		particle.velocity_ = { 
			static_cast<float>(rand() % size - (size - 1) / 2),
			global_->GetValue<float>("PlayerAttackParitcle", "offsetVelocityY"),
			static_cast<float>(rand() % size - (size - 1) / 2) };
		particle.velocity_.Normalize();
		particle.alpha_ = 1.0f;
		combo3Particles_.push_back(std::move(particle));
	}
}
