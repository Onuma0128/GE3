#pragma once
#include <memory>

#include "Object3d.h"
#include "Sprite.h"
#include "Input.h"
#include "Camera.h"

#include "Vector3.h"
#include "Matrix4x4.h"

class RailCamera;

class Reticle3D
{
public:

	void Initialize();

	void Update();

	void Draw();

	void DrawSprite();

	void ScreenToWorld();

	/*==================== アクセッサー ====================*/

	const Vector3 GetPosition() { return reticle3d_->GetPosition(); }

	const Vector3 GetWorldPosition() { return reticle3d_->GetWorldPosition(); }

	void SetRailCamera(RailCamera* railCamera) { railCamera_ = railCamera; }

private:

	std::unique_ptr<Object3d> reticle3d_;
	std::unique_ptr<Sprite> reticleSprite_;

	Camera* camera_ = Camera::GetInstance();
	Input* input_ = Input::GetInstance();
	RailCamera* railCamera_ = nullptr;
};

