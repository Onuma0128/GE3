#pragma once
#include "vector"

#include "Input.h"
#include "Camera.h"
#include "Object3d.h"
#include "Line3d.h"

#include "Vector3.h"
#include "Transform.h"

const float lineNum = 100;

class RailCamera
{
public:

	void Initialize();

	void Update();

	void Draw();

	void DrawLine();

	void Debug_ImGui();

	void RailCameraMove();

	// レールを作る
	void CreateRail();

private:

	Camera* camera_;
	bool isDebug_ = true;
	std::unique_ptr<Object3d> cameraObj_;
	float t_ = 0;

	// カメラが通る点
	std::vector<Vector3> controlPoints_;
	std::unique_ptr<Object3d> newLine_;
	Vector3 newLinePos_{};

	// レールの板ポリ
	std::vector<std::unique_ptr<Object3d>> railObj_;

	// ライン
	std::vector<Vector3> lines;
	std::unique_ptr<Line3d> line3d_;

};

