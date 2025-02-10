#pragma once
#include <memory>

#include "BaseScene.h"

#include "Object3d.h"
#include "WorldTransform.h"
#include "Camera.h"

class GamePlayScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

	void AddQuaternion(Quaternion& quaternion, const Vector3& angle);

private:

	std::unique_ptr<Camera> camera1_ = nullptr;
	std::unique_ptr<Camera> camera2_ = nullptr;

	std::unique_ptr<Object3d> terrain_ = nullptr;
	std::unique_ptr<WorldTransform> terrainTrans_ = nullptr;
	Vector3 terrainAngle_;

	std::unique_ptr<Object3d> sphere_ = nullptr;
	std::unique_ptr<WorldTransform> sphereTrans_ = nullptr;
	Vector3 sphereAngle_;

	std::unique_ptr<Object3d> plane_ = nullptr;
	std::unique_ptr<WorldTransform> planeTrans_ = nullptr;
	Vector3 planeAngle_;

};

