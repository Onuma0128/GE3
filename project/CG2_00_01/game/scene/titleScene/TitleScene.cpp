#include "TitleScene.h"

#include "CameraManager.h"
#include "SceneManager.h"
#include "Input.h"

#include "Object3dBase.h"
#include "SpriteBase.h"
#include "PrimitiveDrawer.h"

#include "Quaternion.h"

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

	Quaternion rotation = Quaternion::MakeRotateAxisAngleQuaternion(Vector3{ 1.0f,0.4f,-0.2f }, 0.45f);
	Vector3 pointY = { 2.1f,-0.9f,1.3f };
	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(rotation);
	Vector3 rotateByQuaternion = Quaternion::RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = pointY.Transform(rotateMatrix);

	ImGui::Begin("MT4");
	rotation.ImGuiQuaternion("rotation");
	Matrix4x4::ImGuiMatrix("MT4", rotateMatrix);
	ImGui::Text("%.3f %.3f %.3f : rotateByQuaternion", rotateByQuaternion.x, rotateByQuaternion.y, rotateByQuaternion.z);
	ImGui::Text("%.3f %.3f %.3f : rotateByMatrix", rotateByMatrix.x, rotateByMatrix.y, rotateByMatrix.z);
	ImGui::End();
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
