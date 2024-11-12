#include "Reticle3D.h"
#include "railCamera/RailCamera.h"

void Reticle3D::Initialize()
{
	// レティクル
	reticle3d_ = std::make_unique<Object3d>("Box.obj");
	reticle3d_->SetScale({ 0.1f,0.1f,0.1f });
	reticle3d_->SetParent(railCamera_->GetCameraObj());

	reticleSprite_ = std::make_unique<Sprite>();
	reticleSprite_->Initialize("2dReticle.png");
	reticleSprite_->SetAnchorPoint({ 0.5f,0.5f });
	reticleSprite_->SetSize({ 64,64 });
}

void Reticle3D::Update()
{
	/// マウスの座標からワールド座標に変換
	ScreenToWorld();

	reticle3d_->Update();

	reticleSprite_->Update();
}

void Reticle3D::Draw()
{
#ifdef _DEBUG
	reticle3d_->Draw();
#endif // _DEBUG
}

void Reticle3D::DrawSprite()
{
	reticleSprite_->Draw();
}

void Reticle3D::ScreenToWorld()
{
	/// マウスの座標を取得
	Vector2 mousePos = { (float)input_->GetMousePosX(), WinApp::kClientHeight - (float)input_->GetMousePosY() };
	reticleSprite_->SetPosition({ mousePos.x,mousePos.y * -1.0f + WinApp::kClientHeight });

	/// ワールド座標への変換行列
	Matrix4x4 matInverseVPV = Inverse(
		camera_->GetProjectionMatrix() *
		camera_->GetViewportMatrix()
	);

	/// ワールド座標へ変換
	Vector3 posNear = Vector3(mousePos.x, mousePos.y, 0);
	Vector3 posFar = Vector3(mousePos.x, mousePos.y, 1);
	posNear = Transform_(posNear, matInverseVPV);
	posFar = Transform_(posFar, matInverseVPV);

	/// 差分ベクトルから3Dレティクルの位置を計算
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);
	const float kDistance = 5.0f;
	reticle3d_->SetPosition(posNear + mouseDirection * kDistance);
}
