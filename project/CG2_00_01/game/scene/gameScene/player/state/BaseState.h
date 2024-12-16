#pragma once

class Player;

class BaseState
{
public:

	BaseState(Player* player);

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void Finalize() = 0;

protected:

	Player* player_ = nullptr;

};