#pragma once
#include "Input.h"

class Player;

class PlayerBase
{
public:
	PlayerBase(Player* player);
	virtual ~PlayerBase() = default;

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

protected:

	Player* player_ = nullptr;

	Input* input_ = Input::GetInstance();

};