#pragma once
#include "vector"

#include "Input.h"
#include "Camera.h"
#include "Object3d.h"
#include "Line3d.h"

#include "Vector3.h"
#include "Transform.h"

#include "bullet/Bullet.h"
#include "reticle/Reticle3D.h"

const float lineNum = 1000;

class RailCamera
{
public:

	void Initialize();

	void Update();

	void Draw();

	void DrawLine();

	void Debug_ImGui();


	// レールを動かす
	void RailCameraMove();
	// レールを作る
	void CreateRail();
	// 弾を作る
	void CreateBullet();

	std::vector<Vector3> GenerateSpiralControlPoints(int numTurns, float radius, float height, int pointsPerTurn);

	Object3d* GetCameraObj() { return cameraObj_.get(); }

private:
	// カメラ
	Camera* camera_ = Camera::GetInstance();
	Input* input_ = Input::GetInstance();

	// カメラの動きが見えるためのオブジェクト
	std::unique_ptr<Object3d> cameraObj_;
	Vector3 offset_ = { 0,1,0 };
	// カメラの動きのスピード
	float t_ = 0;

	// カメラが通る点
	std::vector<Vector3> controlPoints_;
	// レールの板ポリ
	std::vector<std::unique_ptr<Object3d>> railObj_;
	// ラインの座標格納
	std::vector<Vector3> lines;
	// ライン複数描画
	std::unique_ptr<Line3d> line3d_;
	// レティクル
	std::unique_ptr<Reticle3D> reticle3d_;
	// 弾
	std::vector<std::unique_ptr<Bullet>> bullets_;
};

