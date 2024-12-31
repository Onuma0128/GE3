#pragma once

#include <memory>

#include "Object3d.h"
#include "WorldTransform.h"
#include "GlobalVariables.h"

class Player;

class Enemy
{
public:

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