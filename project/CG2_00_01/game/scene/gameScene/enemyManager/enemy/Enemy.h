#pragma once

#include <memory>

#include "Object3d.h"
#include "WorldTransform.h"
#include "GlobalVariables.h"

#include "gameScene/collision/Collider.h"

class Player;

class Enemy : public Collider
{
public:

	void OnCollision(std::string& name)override;
	Vector3 GetCenterPosition() const override;
	std::string GetColliderName() const override;
	float GetRadius()const override;


	~Enemy();

	void Init();

	void Update();

	void Draw();

	void Debug_Update();


	void CollisionEnemy(const Vector3& translation);

	Object3d* GetModel()const { return model_.get(); }
	WorldTransform* GetTransform()const { return transform_.get(); }

	void SetPlayer(Player* player) { player_ = player; }

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();
	Player* player_ = nullptr;

	std::unique_ptr<Object3d> model_ = nullptr;
	std::unique_ptr<WorldTransform> transform_ = nullptr;


};