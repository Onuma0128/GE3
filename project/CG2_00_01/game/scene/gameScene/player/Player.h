#pragma once
#include <memory>
#include <list>

#include "Input.h"
#include "Object3d.h"
#include "WorldTransform.h"
#include "GlobalVariables.h"
#include "ParticleEmitter.h"

#include "Vector3.h"

#include "state/BaseState.h"
#include "gameScene/animation/PlayerAnimation.h"
#include "gameScene/collision/Collider.h"

class Player : public Collider
{
public:

	void OnCollision(std::string& name)override;
	Vector3 GetCenterPosition() const override;
	std::string GetColliderName() const override;
	float GetRadius()const override;


	~Player();

	void Init();

	void Update();

	void ShadowUpdate();

	void Draw();

	void SetIsShake(bool isShake) { isShake_ = isShake; }
	bool GetIsShake()const { return isShake_; }

	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; }
	bool GetIsAttack()const { return isAttack_; }

	PlayerAnimation* GetPlayerAnima()const { return playerAnimation_.get(); }

	/* ==================== モデル&トランスフォーム ==================== */

	Object3d* GetModel()const { return model_.get(); }
	WorldTransform* GetTransform()const { return transform_.get(); }

	Object3d* GetShadowModel()const { return shadowModel_.get(); }
	WorldTransform* GetShadowTransform()const { return shadowTransform_.get(); }

	/* ==================== パーティクル ==================== */

	ParticleEmitter* GetMoveEmitter()const { return moveParticleEmitter_.get(); }
	ParticleEmitter* GetSwordEmitter()const { return swordParticleEmitter_.get(); }

	/* ==================== playerの変数 ==================== */

	const Vector3& GetVelocity() { return velocity_; }
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	const Vector3& GetAcceleration() { return acceleration_; }
	void SetAcceleration(const Vector3& acceleration) { acceleration_ = acceleration; }

	void ChengeState(std::unique_ptr<BaseState> newState);

private:

	void GlobalInit();

private:

	Input* input_ = Input::GetInstance();
	GlobalVariables* global_ = GlobalVariables::GetInstance();

	// モデル
	std::unique_ptr<Object3d> model_ = nullptr;
	std::unique_ptr<WorldTransform> transform_ = nullptr;
	// プレイヤーの影
	std::unique_ptr<Object3d> shadowModel_ = nullptr;
	std::unique_ptr<WorldTransform> shadowTransform_ = nullptr;

	// プレイヤーのモデル
	std::unique_ptr<PlayerAnimation> playerAnimation_ = nullptr;

	// 移動時のパーティクル
	std::unique_ptr<ParticleEmitter> moveParticleEmitter_ = nullptr;

	// 攻撃時剣先のパーティクル
	std::unique_ptr<ParticleEmitter> swordParticleEmitter_ = nullptr;

	// 状態
	std::unique_ptr<BaseState> state_;

	// 変数
	Vector3 velocity_;
	Vector3 acceleration_;
	bool isShake_ = false;
	bool isAttack_ = false;

};