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

	void SetPosition(const Vector3& translation) { transform_->translation_ = translation;; }

	void SetPlayer(Player* player) { player_ = player; }

private:

	Player* player_ = nullptr;

	std::unique_ptr<Object3d> model_ = nullptr;
	std::unique_ptr<WorldTransform> transform_ = nullptr;


};