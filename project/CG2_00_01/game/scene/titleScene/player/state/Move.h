#pragma once
#include <memory>

#include "titleScene/player/state/PlayerBase.h"

#include "Vector3.h"

class Move : public PlayerBase
{
public:

	Move(Player* player);

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:

	float LerpShortAngle(float a, float b, float t);

private:

	Vector3 velocity_;

	float newRotateY = 0;

};

