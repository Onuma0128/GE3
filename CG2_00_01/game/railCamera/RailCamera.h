#pragma once
#include "vector"

#include "Input.h"
#include "Camera.h"
#include "Object3d.h"
#include "Vector3.h"
#include "Transform.h"

class RailCamera
{
public:

	void Initialize();

	void Update();

	void Draw();

	void Debug_ImGui();


	void RailCameraMove();

private:

	Camera* camera_;
	Input* input_;
	std::unique_ptr<Object3d> cameraObj_;
	Transform cameraOffset_{};
	float t_ = 0;

	// カメラが通る点
	std::vector<Vector3> controlPoints_;
	// レールのオブジェクト
	std::vector<std::unique_ptr<Object3d>> railPoints_;
};

