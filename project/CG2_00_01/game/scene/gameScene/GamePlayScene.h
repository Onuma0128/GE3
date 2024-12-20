#pragma once
#include <memory>
#include <vector>

#include "SpriteBase.h"
#include "Sprite.h"
#include "Object3dBase.h"
#include "Object3d.h"
#include "LoadSound.h"
#include "BaseScene.h"
#include "Line3d.h"

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

	std::vector<std::unique_ptr<Object3d>> obj_;

	/* ==================== �J����==================== */

	std::unique_ptr<GameCamera> camera_ = nullptr;

	/* ==================== プレイヤー ==================== */

	std::unique_ptr<Player> player_ = nullptr;


	std::unique_ptr<ParticleEmitter> emitter_ = nullptr;
};

