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
	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	CameraManager::GetInstance()->SetCamera(camera_.get());

	transform_ = std::make_unique<WorldTransform>();

	ModelManager::GetInstance()->LoadModel("resources", "terrain.obj");
	model_ = std::make_unique<Object3d>();
	model_->Initialize("terrain.obj", transform_.get());

	teapotTrans_ = std::make_unique<WorldTransform>();

	ModelManager::GetInstance()->LoadModel("resources", "teapot.obj");
	teapot_ = std::make_unique<Object3d>();
	teapot_->Initialize("teapot.obj", teapotTrans_.get());
	teapotTrans_->parent_ = transform_.get();

}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}

	float joystickX = Input::GetInstance()->GetGamepadLeftStickX();
	float joystickY = Input::GetInstance()->GetGamepadLeftStickY();

	Vector3 velocity = { joystickX, 0.0f, joystickY };
	Vector3 targetDirection = { -joystickX, 0.0f, joystickY };
	Vector3 currentDirection = Vector3::ExprUnitZ;
	if (joystickX != 0.0f || joystickY != 0.0f) {
		Matrix4x4 rotationMatrix = Matrix4x4::DirectionToDirection(currentDirection, targetDirection);
		yRotation = Quaternion::FormRotationMatrix(rotationMatrix);
	}
	teapotTrans_->rotation_.Slerp(yRotation, 0.1f);

	teapotTrans_->translation_ += velocity * 0.1f;

	teapot_->Update();
	model_->Update();

	ImGui::Begin("joystick");
	ImGui::Text("%.3f,%.3f", joystickX, joystickY);
	ImGui::End();
}

void GamePlayScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();
	
	model_->Draw();
	teapot_->Draw();

	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();
	



	// Lineの描画準備
	PrimitiveDrawer::GetInstance()->DrawBase();




	// Particleの描画

}
