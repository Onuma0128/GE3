#pragma once

class Player;
class PlayerAnimation;

class BaseState
{
public:

	BaseState(Player* player,PlayerAnimation* playerAnimation);

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void Finalize() = 0;

protected:

	Player* player_ = nullptr;
	PlayerAnimation* playerAnimation_ = nullptr;

};