#pragma once

#include <memory>

#include "Object3d.h"
#include "WorldTransform.h"
#include "GlobalVariables.h"

#include "gameScene/enemyManager/enemy/state/BaseStateEnemy.h"
#include "gameScene/collision/Collider.h"

class Player;

class Enemy : public Collider
{
public:
	// 衝突判定
	void OnCollision(const std::string& name, const Vector3& position)override;
	Vector3 GetCenterPosition() const override;
	std::string GetColliderName() const override;
	float GetRadius()const override;


	~Enemy();

	void Init();

	void Update();

	void Draw();

	void Debug_Update();

	void CollisionEnemy(const Vector3& translation);

	void ChengeState(std::unique_ptr<BaseStateEnemy> newState);

	int GetHP()const { return hp_; }
	void SetHP(int hp) { hp_ = hp; }

	Object3d* GetModel()const { return model_.get(); }
	WorldTransform* GetTransform()const { return transform_.get(); }

	void SetPlayer(Player* player) { player_ = player; }
	Player* GetPlayer()const { return player_; }

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
	const Vector3& GetVelocity() { return velocity_; }

	void SetIsDamage(bool isDamage) { isDamage_ = isDamage; }

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();
	Player* player_ = nullptr;

	std::unique_ptr<BaseStateEnemy> state_ = nullptr;

	std::unique_ptr<Object3d> model_ = nullptr;
	std::unique_ptr<WorldTransform> transform_ = nullptr;

	Vector3 velocity_;
	int hp_ = 5;
	bool isDamage_ = false;
};