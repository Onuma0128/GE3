#include "TitleScene.h"

#include "CameraManager.h"
#include "SceneManager.h"
#include "Input.h"

#include "Object3dBase.h"
#include "SpriteBase.h"
#include "PrimitiveDrawer.h"

#include "imgui.h"

void TitleScene::Initialize()
{
	// Cameraの初期化
	// ===============
	// Cameraのポインタを初期化
	// CameraManagerに登録
	// ===============
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	CameraManager::GetInstance()->SetCamera(camera_.get());

	camera1_ = std::make_unique<Camera>();
	camera1_->Initialize();
	camera1_->SetRotate(Vector3{ 0.26f,1.57f,0.0f });
	camera1_->SetTranslate(Vector3{ -15.0f,4.0f,0.0f });
	CameraManager::GetInstance()->SetCamera(camera1_.get());
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	if (Input::GetInstance()->PushKey(DIK_F1)) {
		CameraManager::GetInstance()->SetActiveCamera(0);
	}
	if (Input::GetInstance()->PushKey(DIK_F2)) {
		CameraManager::GetInstance()->SetActiveCamera(1);
	}

	Vector3 from0 = { 1.0f,0.7f,0.5f };
	Vector3 to0 = -from0;
	Vector3 from1 = { -0.6f,0.9f,0.2f };
	Vector3 to1 = { 0.4f,0.7f,-0.5f };

	Matrix4x4 rotateMatrix0 = Matrix4x4::DirectionToDirection(
		Vector3{ 1.0f,0.0f,0.0f }, Vector3{ -1.0f,0.0f,0.0f }
	);
	Matrix4x4 rotateMatrix1 = Matrix4x4::DirectionToDirection(from0, to0);
	Matrix4x4 rotateMatrix2 = Matrix4x4::DirectionToDirection(from1, to1);

	Matrix4x4::ImGuiMatrix("MT4", rotateMatrix0);
	Matrix4x4::ImGuiMatrix("MT4", rotateMatrix1);
	Matrix4x4::ImGuiMatrix("MT4", rotateMatrix2);
}

void TitleScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();
	



	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();
	



	// Lineの描画準備
	PrimitiveDrawer::GetInstance()->DrawBase();




	// Particleの描画

}
