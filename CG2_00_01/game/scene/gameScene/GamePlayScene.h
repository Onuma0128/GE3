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
#include "enemy/EnemyManager.h"
#include "collider/CollisionManager.h"


class GamePlayScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

	void CheckAllCollisions();

private:

	std::unique_ptr<Object3d> skydome_;

	std::unique_ptr<RailCamera> railCamera_;

	std::unique_ptr<EnemyManager> enemyManager_;

	std::unique_ptr<CollisionManager> collisionManager_;
};

