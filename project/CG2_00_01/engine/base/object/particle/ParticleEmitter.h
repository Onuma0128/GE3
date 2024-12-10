#pragma once

#include <iostream>
#include <random>
#include <memory>
#include <list>
#include <vector>
#include <unordered_map>

#include "Line3d.h"

#include "Transform.h"
#include "Vector3.h"

#include "ParticleManager.h"
#include "GlobalVariables.h"

class ParitcleManager;

class ParticleEmitter
{
public:

	struct AABB {
		Vector3 min;
		Vector3 max;
	};

	struct Emitter {
		std::string name;
		Transform transform;
		AABB size;
		float scale;
		uint32_t count;
		float frequency;
		float frequencyTime;
	};

	struct AccelerationField {
		Vector3 acceleration;
		AABB area;
	};

public:

	ParticleEmitter(const std::string name);

	void GlobalInitialize(const std::string name);

	void Update();

	void Draw();

	/*==================== パーティクルの生成 ====================*/

	void CreateParticles(ParticleManager::ParticleGroup& group);

	void UpdateParticle(std::list<ParticleManager::Particle>::iterator& particle);

	/*==================== アクセッサー ====================*/

	void SetPosition(const Vector3& position) { emitter_.transform.translate = position; }

	void SetAcceleration(const Vector3& acceleration) { accelerationField_.acceleration = acceleration; }

	void SetIsCreate(bool isCreate) { isCreate_ = isCreate; }

private:

	static std::list<ParticleManager::Particle> Emit(const Emitter& emitter, std::mt19937& randomEngine);

	static ParticleManager::Particle MakeNewParticle(std::mt19937& randomEngine, const Emitter& emitter);

	std::vector<Vector3> CreateLineBox(AABB aabb);

	bool IsCollision(const AABB& aabb, const Vector3& point);

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	/*==================== エミッター ====================*/

	// 乱数生成器の初期化
	std::random_device seedGenerator_;

	// エミッタの範囲を可視化
	std::vector<Vector3> linePosition_;
	std::vector<std::unique_ptr<Line3d>> lines_;

	// Transform変数を作る
	AccelerationField accelerationField_{};
	Emitter emitter_{};
	const float kDeltaTime = 1.0f / 60.0f;
	bool moveStart_ = false;
	bool isFieldStart_ = false;
	bool isCreate_ = true;
};