#pragma once
#include <string>

#include "Vector3.h"

class Collider
{
public:

	virtual ~Collider() = default;
	virtual void OnCollision(std::string& name) {}
	virtual Vector3 GetCenterPosition() const = 0;
	virtual std::string GetColliderName()const = 0;
	virtual float GetRadius()const = 0;

};