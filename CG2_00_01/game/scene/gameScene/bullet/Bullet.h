#pragma once
#include <string>
#include <vector>
#include <memory>

#include "Input.h"
#include "Object3d.h"
#include "Line3d.h"

#include "Vector3.h"

#include "collider/Collider.h"

class RailCamera;

class Bullet : public Collider
{
public:

	void OnCollision()override;
	Vector3 GetCenterPosition() const override;
	std::string GetColliderName() const override;
	float GetRadius()const override;

	void Initialize(const Vector3 position, const Vector3& velocity);

	void Update();

	void Draw();

	void LineDraw();

	void Move();

	bool GetIsActive()const { return isActive_; }

	void SetRailCamera(RailCamera* camera) { camera_ = camera; }

private:
	RailCamera* camera_ = nullptr;

	std::unique_ptr<Object3d> bullet_;
	Vector3 velocity_;

	float bulletFrame_ = 0;
	bool isActive_;

	std::unique_ptr<Line3d> line_;
	Vector3 lineStartPos_;
};