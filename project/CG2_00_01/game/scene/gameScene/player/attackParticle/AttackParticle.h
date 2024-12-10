#pragma once
#include <memory>
#include <string>

#include "Object3d.h"
#include "Vector3.h"
#include "GlobalVariables.h"

class AttackParticle
{
public:

	void Init(const std::string& modelName, const Vector3& position);

	void Update();

	void Draw();

	bool GetIsActive()const { return isActive_; }

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	std::unique_ptr<Object3d> particle_;
	Vector3 velocity_;
	float alpha_ = 1.0f;

	bool isActive_ = false;
};