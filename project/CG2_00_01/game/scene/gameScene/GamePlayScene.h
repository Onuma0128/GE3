#pragma once
#include <memory>
#include <vector>

#include "BaseScene.h"

#include "WorldTransform.h"
#include "Object3d.h"
#include "ParticleEmitter.h"

#include "gameCamera/GameCamera.h"
#include "player/Player.h"
#include "enemyManager/EnemyManager.h"
#include "collision/CollisionManager.h"

class GamePlayScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

	void CheckAllCollisions();

private:
	/* ==================== マップ ==================== */

	std::unique_ptr<Object3d> ground_;
	std::unique_ptr<WorldTransform> groundTransform_;
	std::unique_ptr<ParticleEmitter> emitter_ = nullptr;

	/* ==================== 天球 ==================== */

	std::unique_ptr<Object3d> skydome_;
	std::unique_ptr<WorldTransform> skydomeTransform_;

	/* ==================== カメラ ==================== */

	std::unique_ptr<GameCamera> camera_ = nullptr;

	/* ==================== プレイヤー ==================== */

	std::unique_ptr<Player> player_ = nullptr;

	/* ==================== 敵のマネージャー ==================== */

	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

	/* ==================== 衝突マネージャー ==================== */

	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

};

