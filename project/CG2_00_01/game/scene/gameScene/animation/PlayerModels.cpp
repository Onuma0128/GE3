#include "PlayerModels.h"

#include "Quaternion.h"

#include "gameScene/player/Player.h"

void PlayerModels::Init()
{
	GlobalInit();

	// 体
	bodyTrans_ = std::make_unique<WorldTransform>();
	body_ = std::make_unique<Object3d>();
	body_->Initialize("player_body.obj", bodyTrans_.get());
	models_.push_back(std::move(body_));
	// 頭
	headTrans_ = std::make_unique<WorldTransform>();
	head_ = std::make_unique<Object3d>();
	head_->Initialize("player_head.obj", headTrans_.get());
	models_.push_back(std::move(head_));
	// 左肩
	leftShoulderTrans_ = std::make_unique<WorldTransform>();
	leftShoulder_ = std::make_unique<Object3d>();
	leftShoulder_->Initialize("player_leftShoulder.obj", leftShoulderTrans_.get());
	models_.push_back(std::move(leftShoulder_));
	// 右肩
	rightShoulderTrans_ = std::make_unique<WorldTransform>();
	rightShoulder_ = std::make_unique<Object3d>();
	rightShoulder_->Initialize("player_rightShoulder.obj", rightShoulderTrans_.get());
	models_.push_back(std::move(rightShoulder_));
	// 左腕
	leftArmTrans_ = std::make_unique<WorldTransform>();
	leftArm_ = std::make_unique<Object3d>();
	leftArm_->Initialize("player_leftArm.obj", leftArmTrans_.get());
	models_.push_back(std::move(leftArm_));
	// 右腕
	rightArmTrans_ = std::make_unique<WorldTransform>();
	rightArm_ = std::make_unique<Object3d>();
	rightArm_->Initialize("player_rightArm.obj", rightArmTrans_.get());
	models_.push_back(std::move(rightArm_));
	// 剣
	swordTrans_ = std::make_unique<WorldTransform>();
	sword_ = std::make_unique<Object3d>();
	sword_->Initialize("sword.obj", swordTrans_.get());
	models_.push_back(std::move(sword_));

	// ペアレント
	bodyTrans_->parent_ = player_->GetTransform();
	headTrans_->parent_ = bodyTrans_.get();
	leftShoulderTrans_->parent_ = bodyTrans_.get();
	rightShoulderTrans_->parent_ = bodyTrans_.get();
	leftArmTrans_->parent_ = leftShoulderTrans_.get();
	rightArmTrans_->parent_ = rightShoulderTrans_.get();
	swordTrans_->parent_ = rightArmTrans_.get();

	ModelOffset();
}

void PlayerModels::GlobalInit()
{
	global_->AddValue<Vector3>("PlayerModelOffset", "head", Vector3{});
	global_->AddValue<Vector3>("PlayerModelOffset", "body", Vector3{});
	global_->AddValue<Vector3>("PlayerModelOffset", "leftShoulder", Vector3{});
	global_->AddValue<Vector3>("PlayerModelOffset", "rightShoulder", Vector3{});
	global_->AddValue<Vector3>("PlayerModelOffset", "leftArm", Vector3{});
	global_->AddValue<Vector3>("PlayerModelOffset", "rightArm", Vector3{});
	global_->AddValue<Vector3>("PlayerModelOffset", "sword", Vector3{});

	global_->AddValue<Vector3>("PlayerModelOffset", "headAngle", Vector3{});
	global_->AddValue<Vector3>("PlayerModelOffset", "bodyAngle", Vector3{});
	global_->AddValue<Vector3>("PlayerModelOffset", "leftShoulderAngle", Vector3{});
	global_->AddValue<Vector3>("PlayerModelOffset", "rightShoulderAngle", Vector3{});
	global_->AddValue<Vector3>("PlayerModelOffset", "leftArmAngle", Vector3{});
	global_->AddValue<Vector3>("PlayerModelOffset", "rightArmAngle", Vector3{});
	global_->AddValue<Vector3>("PlayerModelOffset", "swordAngle", Vector3{});

}

void PlayerModels::Update()
{
	for (auto& model : models_) {
		model->Update();
	}
}

void PlayerModels::Draw()
{
	for (auto& model : models_) {
		model->Draw();
	}
}

void PlayerModels::ModelOffset()
{
	// 回転指定
	RotationQuaternion(headTrans_->rotation_, global_->GetValue<Vector3>("PlayerModelOffset", "headAngle"));
	RotationQuaternion(bodyTrans_->rotation_, global_->GetValue<Vector3>("PlayerModelOffset", "bodyAngle"));
	RotationQuaternion(leftShoulderTrans_->rotation_, global_->GetValue<Vector3>("PlayerModelOffset", "leftShoulderAngle"));
	RotationQuaternion(rightShoulderTrans_->rotation_, global_->GetValue<Vector3>("PlayerModelOffset", "rightShoulderAngle"));
	RotationQuaternion(leftArmTrans_->rotation_, global_->GetValue<Vector3>("PlayerModelOffset", "leftArmAngle"));
	RotationQuaternion(rightArmTrans_->rotation_, global_->GetValue<Vector3>("PlayerModelOffset", "rightArmAngle"));
	RotationQuaternion(swordTrans_->rotation_, global_->GetValue<Vector3>("PlayerModelOffset", "swordAngle"));
	// 座標指定
	headTrans_->translation_ = global_->GetValue<Vector3>("PlayerModelOffset", "head");
	bodyTrans_->translation_ = global_->GetValue<Vector3>("PlayerModelOffset", "body");
	leftShoulderTrans_->translation_ = global_->GetValue<Vector3>("PlayerModelOffset", "leftShoulder");
	rightShoulderTrans_->translation_ = global_->GetValue<Vector3>("PlayerModelOffset", "rightShoulder");
	leftArmTrans_->translation_ = global_->GetValue<Vector3>("PlayerModelOffset", "leftArm");
	rightArmTrans_->translation_ = global_->GetValue<Vector3>("PlayerModelOffset", "rightArm");
	swordTrans_->translation_ = global_->GetValue<Vector3>("PlayerModelOffset", "sword");
}

void PlayerModels::RotationQuaternion(Quaternion& q, const Vector3& v)
{
	Quaternion rotationX = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, v.x);
	Quaternion rotationY = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, v.y);
	Quaternion rotationZ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitZ, v.z);

	q = (rotationX * rotationY * rotationZ);
}