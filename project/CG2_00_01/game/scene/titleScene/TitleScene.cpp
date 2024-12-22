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

	Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
	Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };

	Quaternion identity = Quaternion::IdentityQuaternion();
	Quaternion conj = q1;
	Quaternion inv = q1;
	Quaternion normal = q1;
	Quaternion mul1 = q1 * q2;
	Quaternion mul2 = q2 * q1;
	float norm = q1.Norm();

	ImGui::Begin("MT4");
	identity.ImGuiQuaternion("Identity\n");
	conj.Conjugate().ImGuiQuaternion("Conjugate\n");
	inv.Inverse().ImGuiQuaternion("Inverse\n");
	normal.Normalize().ImGuiQuaternion("Normalize\n");
	mul1.ImGuiQuaternion("Multiply(q1, q2)\n");
	mul2.ImGuiQuaternion("Multiply(q2, q1)\n");
	ImGui::Text("%.3f : Norm", norm);
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
