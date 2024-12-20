#pragma once

#include <memory>

#include "Sprite.h"
#include "Vector2.h"

class Player
{
public:

	void Init();

	void Update();

	void Draw();

	void MoveRight();
	void MoveLeft();

private:

	std::unique_ptr<Sprite> sprite_ = nullptr;

	Vector2 position_ = {};
	const float speed_ = 1.0f;

};

