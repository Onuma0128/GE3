#pragma once
#include <string>

#include "Vector3.h"

class Collider
{
public:

	virtual ~Collider() = default;
	virtual void OnCollision(const std::string& name, const Vector3& position) {}
	virtual Vector3 GetCenterPosition() const = 0;
	virtual std::string GetColliderName()const = 0;
	virtual float GetRadius()const = 0;

};