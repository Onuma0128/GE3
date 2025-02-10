#include "GamePlayScene.h"

#include "CameraManager.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "Input.h"

#include "Object3dBase.h"
#include "SpriteBase.h"
#include "PrimitiveDrawer.h"

#include "imgui.h"
#include "titleScene/TitleScene.h"

void GamePlayScene::Initialize()
{
	//// カメラの初期化
	camera1_ = std::make_unique<Camera>();
	camera1_->Initialize();
	camera1_->SetTranslation({ -10.0f,4.0f,-15.0f });
	camera2_ = std::make_unique<Camera>();
	camera2_->Initialize();
	camera2_->SetTranslation({ 10.0f,4.0f,-15.0f });
	CameraManager::GetInstance()->SetCamera(camera1_.get());
	CameraManager::GetInstance()->SetCamera(camera2_.get());

	terrainTrans_ = std::make_unique<WorldTransform>();
	ModelManager::GetInstance()->LoadModel("resources", "terrain.obj");
	terrain_ = std::make_unique<Object3d>();
	terrain_->Initialize("terrain.obj", terrainTrans_.get());

	sphereTrans_ = std::make_unique<WorldTransform>();
	ModelManager::GetInstance()->LoadModel("resources", "sphere.obj");
	sphere_ = std::make_unique<Object3d>();
	sphere_->Initialize("sphere.obj", sphereTrans_.get());
	sphere_->SetTexture("resources","uvChecker.png");
	sphereTrans_->translation_ = { 1.5f,1.0f,0.0f };

	planeTrans_ = std::make_unique<WorldTransform>();
	ModelManager::GetInstance()->LoadModel("resources", "plane.gltf");
	plane_ = std::make_unique<Object3d>();
	plane_->Initialize("plane.gltf", planeTrans_.get());
	planeTrans_->translation_ = { -1.5f,1.0f,0.0f };
	planeAngle_.y = 3.14f;

}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();
	CameraManager* cameraManager = CameraManager::GetInstance();
	if (input->TriggerKey(DIK_0)) {
		cameraManager->SetActiveCamera(0);
	}
	if (input->TriggerKey(DIK_1)) {
		cameraManager->SetActiveCamera(1);
	}
	if (input->TriggerKey(DIK_2)) {
		cameraManager->SetActiveCamera(2);
	}

	ImGui::Begin("models");
	ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::TreeNodeEx("objectTransform", flag)) {
		if (ImGui::TreeNodeEx("terrain", flag)) {
			ImGui::DragFloat3("scale", &terrainTrans_->scale_.x, 0.01f);
			ImGui::DragFloat3("rotation", &terrainAngle_.x, 0.01f);
			ImGui::DragFloat3("translation", &terrainTrans_->translation_.x, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("sphere", flag)) {
			ImGui::DragFloat3("scale", &sphereTrans_->scale_.x, 0.01f);
			ImGui::DragFloat3("rotation", &sphereAngle_.x, 0.01f);
			ImGui::DragFloat3("translation", &sphereTrans_->translation_.x, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("plane", flag)) {
			ImGui::DragFloat3("scale", &planeTrans_->scale_.x, 0.01f);
			ImGui::DragFloat3("rotation", &planeAngle_.x, 0.01f);
			ImGui::DragFloat3("translation", &planeTrans_->translation_.x, 0.01f);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::End();

	// 回転を計算
	AddQuaternion(terrainTrans_->rotation_, terrainAngle_);
	AddQuaternion(sphereTrans_->rotation_, sphereAngle_);
	AddQuaternion(planeTrans_->rotation_, planeAngle_);

	terrain_->Update();
	sphere_->Update();
	plane_->Update();
}

void GamePlayScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();
	
	terrain_->Draw();
	sphere_->Draw();
	plane_->Draw();

	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();
	



	// Lineの描画準備
	PrimitiveDrawer::GetInstance()->DrawBase();




	// Particleの描画

}

void GamePlayScene::AddQuaternion(Quaternion& quaternion, const Vector3& angle)
{
	Quaternion addQuaternionX = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, angle.x);
	Quaternion addQuaternionY = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, angle.y);
	Quaternion addQuaternionZ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitZ, angle.z);
	quaternion = (addQuaternionX * addQuaternionY * addQuaternionZ);
}
