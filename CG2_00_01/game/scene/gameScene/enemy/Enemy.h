#pragma once

#include <memory>
#include <string>

#include "Object3d.h"

#include "Vector3.h"

class Enemy
{
public:

	enum class State {
		Move,
		Dead
	};

	Enemy(const std::string& filePath);

	void Initialize(const Vector3& position,const Vector3& velocity);

	void Update();

	void Draw();

private:

	std::string filePath_;

	std::unique_ptr<Object3d> enemy_ = nullptr;
	Vector3 velocity_;

	State state_;

};