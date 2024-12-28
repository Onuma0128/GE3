#pragma once
#include <memory>
#include <vector>

#include "Object3d.h"
#include "WorldTransform.h"
#include "GlobalVariables.h"

class Player;

class PlayerModels
{
public:

	void Init();

	void GlobalInit();

	void Update();

	void Draw();

	// モデルの初期位置
	void ModelOffset();
	// 回転計算
	void RotationQuaternion(Quaternion& q, const Vector3& v);
	// プレイヤーのポインタを取得
	void SetPlayer(Player* player) { player_ = player; }

	// Modelのゲッター
	Object3d* GetBodyModel() { return body_.get(); }
	Object3d* GetLeftShoulderModel() { return leftShoulder_.get(); }
	Object3d* GetRightShoulderModel() { return rightShoulder_.get(); }
	Object3d* GetLeftArmModel() { return leftArm_.get(); }
	Object3d* GetRightArmModel() { return rightArm_.get(); }
	std::vector<std::unique_ptr<Object3d>>& GetModels() { return models_; }

	// Transformのゲッター
	WorldTransform* GetBodyTrans() { return bodyTrans_.get(); }
	WorldTransform* GetLeftShoulderTrans() { return leftShoulderTrans_.get(); }
	WorldTransform* GetRightShoulderTrans() { return rightShoulderTrans_.get(); }
	WorldTransform* GetLeftArmTrans() { return leftArmTrans_.get(); }
	WorldTransform* GetRightArmTrans() { return rightArmTrans_.get(); }

private:

	GlobalVariables* global_ = GlobalVariables::GetInstance();

	Player* player_ = nullptr;

	/* ==================== モデル ==================== */

	std::unique_ptr<Object3d> head_ = nullptr;
	std::unique_ptr<Object3d> body_ = nullptr;
	std::unique_ptr<Object3d> leftShoulder_ = nullptr;
	std::unique_ptr<Object3d> rightShoulder_ = nullptr;
	std::unique_ptr<Object3d> leftArm_ = nullptr;
	std::unique_ptr<Object3d> rightArm_ = nullptr;
	std::unique_ptr<Object3d> sword_ = nullptr;

	std::vector<std::unique_ptr<Object3d>> models_;

	/* ==================== トランスフォーム ==================== */

	std::unique_ptr<WorldTransform> headTrans_ = nullptr;
	std::unique_ptr<WorldTransform> bodyTrans_ = nullptr;
	std::unique_ptr<WorldTransform> leftShoulderTrans_ = nullptr;
	std::unique_ptr<WorldTransform> rightShoulderTrans_ = nullptr;
	std::unique_ptr<WorldTransform> leftArmTrans_ = nullptr;
	std::unique_ptr<WorldTransform> rightArmTrans_ = nullptr;
	std::unique_ptr<WorldTransform> swordTrans_ = nullptr;

};