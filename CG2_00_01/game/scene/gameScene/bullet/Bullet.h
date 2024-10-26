#pragma once
#include <vector>
#include <memory>

#include "Input.h"
#include "Object3d.h"

#include "Vector3.h"

class Bullet
{
public:

	void Initialize(const Vector3 position, const Vector3& velocity);

	void Update();

	void Draw();

	void Move();

	bool GetIsActive()const { return isActive_; }

private:

	std::unique_ptr<Object3d> bullet_;
	Vector3 velocity_;

	float bulletFrame_ = 0;
	bool isActive_;
};