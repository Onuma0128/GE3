#pragma once
#include "SpriteBase.h"

class Sprite
{
public:

	void Initialize(SpriteBase* spriteBase);


private:

	SpriteBase* spriteBase_ = nullptr;
};

