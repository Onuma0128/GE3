#pragma once
#include <memory>
#include <vector>

#include "BaseScene.h"

#include "WorldTransform.h"
#include "Object3d.h"
#include "ParticleEmitter.h"

#include "gameCamera/GameCamera.h"
#include "player/Player.h"

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

	/* ==================== �J���� ==================== */

	std::unique_ptr<GameCamera> camera_ = nullptr;

	/* ==================== �v���C���[ ==================== */

	std::unique_ptr<Player> player_ = nullptr;


	std::unique_ptr<ParticleEmitter> emitter_ = nullptr;
};

