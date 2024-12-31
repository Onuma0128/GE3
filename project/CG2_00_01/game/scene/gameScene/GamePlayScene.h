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

class GamePlayScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	std::unique_ptr<Object3d> ground_;
	std::unique_ptr<WorldTransform> groundTransform_;

	/* ==================== カメラ ==================== */

	std::unique_ptr<GameCamera> camera_ = nullptr;

	/* ==================== プレイヤー ==================== */

	std::unique_ptr<Player> player_ = nullptr;

	/* ==================== 敵のマネージャー ==================== */

	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;


	std::unique_ptr<ParticleEmitter> emitter_ = nullptr;
};

