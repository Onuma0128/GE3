#pragma once
#include <memory>

#include "Sprite.h"

#include "Vector2.h"

class Fade
{
public:

	void Initialize();

	void Update();

	void Draw();

private:
	std::unique_ptr<Sprite> sprite_;

	float time_ = 0;

};