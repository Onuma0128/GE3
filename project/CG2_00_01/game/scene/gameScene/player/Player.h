#pragma once
#include <memory>
#include <list>

#include "Audio.h"
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "GlobalVariables.h"
#include "ParticleEmitter.h"

#include "Vector3.h"

#include "state/BaseState.h"
#include "gameScene/animation/PlayerAnimation.h"
#include "gameScene/collision/Collider.h"
#include "gameScene/player/ui/PlayerUI.h"

class Player : public Collider
{
public:
	// 衝突判定
	void OnCollision(const std::string& name, const Vector3& position)override;
	Vector3 GetCenterPosition() const override;
	std::string GetColliderName() const override;
	float GetRadius()const override;


	~Player();

	void Init();

	void Update();

	void ShadowUpdate();

	void Draw();
	void DrawSprite();

	void IsDamage();

	// カメラシェイク
	void SetIsShake(bool isShake) { isShake_ = isShake; }
	bool GetIsShake()const { return isShake_; }

	// 今攻撃中か
	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; }
	bool GetIsAttack()const { return isAttack_; }

	int GetHP()const { return hp_; }
	void SetHP(int hp) { hp_ = hp; }

	Audio* GetAudio()const { return audio_.get(); }
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
	std::unique_ptr<Audio> audio_ = nullptr;

	// モデル
	std::unique_ptr<Object3d> model_ = nullptr;
	std::unique_ptr<WorldTransform> transform_ = nullptr;
	// プレイヤーの影
	std::unique_ptr<Object3d> shadowModel_ = nullptr;
	std::unique_ptr<WorldTransform> shadowTransform_ = nullptr;
	// プレイヤーのモデル
	std::unique_ptr<PlayerAnimation> playerAnimation_ = nullptr;
	// プレイヤーのUI
	std::unique_ptr<PlayerUI> playerUI_ = nullptr;
	// 状態
	std::unique_ptr<BaseState> state_;


	// 移動時のパーティクル
	std::unique_ptr<ParticleEmitter> moveParticleEmitter_ = nullptr;
	// 攻撃時剣先のパーティクル
	std::unique_ptr<ParticleEmitter> swordParticleEmitter_ = nullptr;

	// 変数
	int hp_ = 5;
	Vector3 velocity_;
	Vector3 acceleration_;
	// カメラシェイク
	bool isShake_ = false;
	// 攻撃中か
	bool isAttack_ = false;

};