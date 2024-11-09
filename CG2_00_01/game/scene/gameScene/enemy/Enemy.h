#pragma once

#include <memory>
#include <string>

#include "Object3d.h"

#include "Vector3.h"
#include "collider/Collider.h"

class Enemy : public Collider
{
public:

	enum class State {
		Move,
		Dead
	};

	void OnCollision()override;
	Vector3 GetCenterPosition() const override;
	std::string GetColliderName() const override;
	float GetRadius()const override;

	void Initialize(const Vector3& position,const Vector3& velocity);

	void Update();

	void Draw();

	void SetFilePath(const std::string& filePath);

	State GetState()const { return state_; }

	bool GetIsActive()const { return isActive_; }

	Object3d* GetEnemyObject()const { return enemy_.get(); }

private:

	std::string filePath_;

	std::unique_ptr<Object3d> enemy_ = nullptr;
	Vector3 velocity_;

	State state_;

	bool isActive_ = true;
	float activeFrame_ = 0;

};