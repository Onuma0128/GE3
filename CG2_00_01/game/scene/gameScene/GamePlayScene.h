#pragma once
#include <memory>
#include <vector>
#include "wrl.h"

#include "SpriteBase.h"
#include "Sprite.h"
#include "Object3dBase.h"
#include "Object3d.h"
#include "BaseScene.h"
#include "Line3d.h"
#include "Camera.h"

#include "railCamera/RailCamera.h"
#include "bullet/Bullet.h"


using Microsoft::WRL::ComPtr;

class GamePlayScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	std::unique_ptr<Object3d> ground_;

	std::unique_ptr<RailCamera> railCamera_;

};

