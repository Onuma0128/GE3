#include "PlayerAnimation.h"

#include "imgui.h"

void PlayerAnimation::Init()
{
	GlovalInit();

	playerModels_ = std::make_unique<PlayerModels>();
	playerModels_->SetPlayer(player_);
	playerModels_->Init();
}

void PlayerAnimation::Update()
{
	playerModels_->Update();
}

void PlayerAnimation::Draw()
{
	playerModels_->Draw();
}

void PlayerAnimation::GlovalInit()
{
	// 通常時アニメーション
	global_->AddValue<float>("NormalAnimation", "frame", 60.0f);

	// 攻撃時アニメーション(1コンボ)
	global_->AddValue<int>("AttackCombo1", "frameNum", 0);
	
	// 攻撃時アニメーション(2コンボ)
	global_->AddValue<int>("AttackCombo2", "frameNum", 0);
	
	// 攻撃時アニメーション(3コンボ)
	global_->AddValue<int>("AttackCombo3", "frameNum", 0);
}

void PlayerAnimation::Reset()
{
	normalFrame_ = 0.0f;
	combo1Frame_ = 0.0f;
	combo2Frame_ = 0.0f;
	combo3Frame_ = 0.0f;

	nextCombo_ = false;

	combo1Completion_ = false;
	combo2Completion_ = false;
	combo3Completion_ = false;
}

void PlayerAnimation::Debug_ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("AddAttackFrame");
	// 1コンボ目のフレームを追加
	int newFrame = global_->GetValue<int>("AttackCombo1", "frameNum");
	if (ImGui::Button("AttackCombo1_NewFrame")) {
		newFrame += 1;
		global_->SetValue<int>("AttackCombo1", "frameNum", newFrame);
		AddGlobalVariables(1, newFrame);
	}
	ImGui::Text("%f", combo1Frame_);
	
	// 2コンボ目のフレームを追加
	newFrame = global_->GetValue<int>("AttackCombo2", "frameNum");
	if (ImGui::Button("AttackCombo2_NewFrame")) {
		newFrame += 1;
		global_->SetValue<int>("AttackCombo2", "frameNum", newFrame);
		AddGlobalVariables(2, newFrame);
	}

	// 3コンボ目のフレームを追加
	newFrame = global_->GetValue<int>("AttackCombo3", "frameNum");
	if (ImGui::Button("AttackCombo3_NewFrame")) {
		newFrame += 1;
		global_->SetValue<int>("AttackCombo3", "frameNum", newFrame);
		AddGlobalVariables(3, newFrame);
	}

	ImGui::End();
#endif // _DEBUG

}

float PlayerAnimation::LerpShortAngleExtended(float a, float b, float t)
{
	float diff = b - a;

	// 角度を [-10, +10] に補正する
	while (diff > 30.0f) {
		diff -= 30.0f * 2.0f;
	}
	while (diff < -30.0f) {
		diff += 30.0f * 2.0f;
	}

	// 角度を [-5, +5] に補正する
	if (diff > 15.0f) {
		diff -= 30.0f;
	}
	else if (diff < -15.0f) {
		diff += 30.0f;
	}

	// 線形補間を行う
	return a + t * diff;
}

void PlayerAnimation::Vector3LerpShortPosition(Vector3& v1, const Vector3& v2, float t)
{
	v1.x = LerpShortAngleExtended(v1.x, v2.x, t);
	v1.y = LerpShortAngleExtended(v1.y, v2.y, t);
	v1.z = LerpShortAngleExtended(v1.z, v2.z, t);
}

void PlayerAnimation::RotationQuaternion(Quaternion& q, const Vector3& v)
{
	q = Quaternion::IdentityQuaternion();
	Quaternion rotationX = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, v.x);
	Quaternion rotationY = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, v.y);
	Quaternion rotationZ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitZ, v.z);

	q = (rotationX * rotationY * rotationZ);
}

void PlayerAnimation::AddGlobalVariables(int comboNum, int frameNum)
{
	// 現在のコンボを取得
	std::string AttackCombo = "AttackCombo" + std::to_string(comboNum);
	// 追加するフレーム何個目かを取得
	std::string num = std::to_string(frameNum);

	// 何フレームアニメーションするか
	global_->AddValue<float>(AttackCombo, "frame" + num, 60.0f);
	// アニメーションのスラープの速度
	global_->AddValue<float>(AttackCombo, "slerpSpeed" + num, 0.3f);

	// 回転用
	global_->AddValue<Vector3>(AttackCombo, "headAngle" + num, Vector3{});
	global_->AddValue<Vector3>(AttackCombo, "bodyAngle" + num, Vector3{});
	global_->AddValue<Vector3>(AttackCombo, "leftShoulderAngle" + num, Vector3{});
	global_->AddValue<Vector3>(AttackCombo, "rightShoulderAngle" + num, Vector3{});
	global_->AddValue<Vector3>(AttackCombo, "leftArmAngle" + num, Vector3{});
	global_->AddValue<Vector3>(AttackCombo, "rightArmAngle" + num, Vector3{});
	global_->AddValue<Vector3>(AttackCombo, "swordAngle" + num, Vector3{});

	// 座標用
	global_->AddValue<Vector3>(AttackCombo, "headPos" + num, Vector3{});
	global_->AddValue<Vector3>(AttackCombo, "bodyPos" + num, Vector3{});
	global_->AddValue<Vector3>(AttackCombo, "leftShoulderPos" + num, Vector3{});
	global_->AddValue<Vector3>(AttackCombo, "rightShoulderPos" + num, Vector3{});
	global_->AddValue<Vector3>(AttackCombo, "leftArmPos" + num, Vector3{});
	global_->AddValue<Vector3>(AttackCombo, "rightArmPos" + num, Vector3{});
	global_->AddValue<Vector3>(AttackCombo, "swordPos" + num, Vector3{});
}

void PlayerAnimation::AnimationUpdate(int comboNum, int frameNum)
{
	// 現在のコンボを取得
	std::string AttackCombo = "AttackCombo" + std::to_string(comboNum);
	// 追加するフレーム何個目かを取得
	std::string num = std::to_string(frameNum);

	// Slerp回転用Quaternion
	Quaternion offsetQ = Quaternion::IdentityQuaternion();
	Quaternion slerpQ = Quaternion::IdentityQuaternion();
	Vector3 slerpV = Vector3::ExprZero;
	float slerpSpeed = global_->GetValue<float>(AttackCombo, "slerpSpeed" + num);

	/* ========== 頭 ========== */
	// 回転
	RotationQuaternion(offsetQ, global_->GetValue<Vector3>("PlayerModelOffset", "headAngle"));
	RotationQuaternion(slerpQ, global_->GetValue<Vector3>(AttackCombo, "headAngle" + num));
	playerModels_->GetHeadTrans()->rotation_.Slerp((offsetQ * slerpQ), slerpSpeed);
	// 座標
	slerpV = global_->GetValue<Vector3>(AttackCombo, "headPos" + num) + global_->GetValue<Vector3>("PlayerModelOffset", "head");
	Vector3LerpShortPosition(playerModels_->GetHeadTrans()->translation_, slerpV, slerpSpeed);

	/* ========== 体 ========== */
	// 回転
	RotationQuaternion(offsetQ, global_->GetValue<Vector3>("PlayerModelOffset", "bodyAngle"));
	RotationQuaternion(slerpQ, global_->GetValue<Vector3>(AttackCombo, "bodyAngle" + num));
	playerModels_->GetBodyTrans()->rotation_.Slerp((offsetQ * slerpQ), slerpSpeed);
	// 座標
	slerpV = global_->GetValue<Vector3>(AttackCombo, "bodyPos" + num) + global_->GetValue<Vector3>("PlayerModelOffset", "body");
	Vector3LerpShortPosition(playerModels_->GetBodyTrans()->translation_, slerpV, slerpSpeed);

	/* ========== 左肩 ========== */
	// 回転
	RotationQuaternion(offsetQ, global_->GetValue<Vector3>("PlayerModelOffset", "leftShoulderAngle"));
	RotationQuaternion(slerpQ, global_->GetValue<Vector3>(AttackCombo, "leftShoulderAngle" + num));
	playerModels_->GetLeftShoulderTrans()->rotation_.Slerp((offsetQ * leftShoulder_Quat_ * slerpQ), slerpSpeed);
	// 座標
	slerpV = global_->GetValue<Vector3>(AttackCombo, "leftShoulderPos" + num) + global_->GetValue<Vector3>("PlayerModelOffset", "leftShoulder");
	Vector3LerpShortPosition(playerModels_->GetLeftShoulderTrans()->translation_, slerpV, slerpSpeed);

	/* ========== 右肩 ========== */
	// 回転
	RotationQuaternion(offsetQ, global_->GetValue<Vector3>("PlayerModelOffset", "rightShoulderAngle"));
	RotationQuaternion(slerpQ, global_->GetValue<Vector3>(AttackCombo, "rightShoulderAngle" + num));
	playerModels_->GetRightShoulderTrans()->rotation_.Slerp((offsetQ * rightShoulder_Quat_ * slerpQ), slerpSpeed);
	// 座標
	slerpV = global_->GetValue<Vector3>(AttackCombo, "rightShoulderPos" + num) + global_->GetValue<Vector3>("PlayerModelOffset", "rightShoulder");
	Vector3LerpShortPosition(playerModels_->GetRightShoulderTrans()->translation_, slerpV, slerpSpeed);

	/* ========== 左腕 ========== */
	// 回転
	RotationQuaternion(offsetQ, global_->GetValue<Vector3>("PlayerModelOffset", "leftArmAngle"));
	RotationQuaternion(slerpQ, global_->GetValue<Vector3>(AttackCombo, "leftArmAngle" + num));
	playerModels_->GetLeftArmTrans()->rotation_.Slerp((offsetQ * slerpQ), slerpSpeed);
	// 座標
	slerpV = global_->GetValue<Vector3>(AttackCombo, "leftArmPos" + num) + global_->GetValue<Vector3>("PlayerModelOffset", "leftArm");
	Vector3LerpShortPosition(playerModels_->GetLeftArmTrans()->translation_, slerpV, slerpSpeed);

	/* ========== 右腕 ========== */
	// 回転
	RotationQuaternion(offsetQ, global_->GetValue<Vector3>("PlayerModelOffset", "rightArmAngle"));
	RotationQuaternion(slerpQ, global_->GetValue<Vector3>(AttackCombo, "rightArmAngle" + num));
	playerModels_->GetRightArmTrans()->rotation_.Slerp((offsetQ * slerpQ), slerpSpeed);
	// 座標
	slerpV = global_->GetValue<Vector3>(AttackCombo, "rightArmPos" + num) + global_->GetValue<Vector3>("PlayerModelOffset", "rightArm");
	Vector3LerpShortPosition(playerModels_->GetRightArmTrans()->translation_, slerpV, slerpSpeed);

	/* ========== 剣 ========== */
	// 回転
	RotationQuaternion(offsetQ, global_->GetValue<Vector3>("PlayerModelOffset", "swordAngle"));
	RotationQuaternion(slerpQ, global_->GetValue<Vector3>(AttackCombo, "swordAngle" + num));
	playerModels_->GetSwordTrans()->rotation_.Slerp((offsetQ * slerpQ), slerpSpeed);
	// 座標
	slerpV = global_->GetValue<Vector3>(AttackCombo, "swordPos" + num) + global_->GetValue<Vector3>("PlayerModelOffset", "sword");
	Vector3LerpShortPosition(playerModels_->GetSwordTrans()->translation_, slerpV, slerpSpeed);

}

void PlayerAnimation::NormalAnimation()
{
	Quaternion offsetQ = Quaternion::IdentityQuaternion();
	normalFrame_ += 1.0f / global_->GetValue<float>("NormalAnimation", "frame");
	float sin = std::sin(normalFrame_) * 0.1f;

	// 体を浮遊させる
	playerModels_->GetBodyTrans()->translation_.y = global_->GetValue<Vector3>("PlayerModelOffset", "body").y + sin;
	// 左肩を回転
	leftShoulder_Quat_ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, sin);
	RotationQuaternion(offsetQ, global_->GetValue<Vector3>("PlayerModelOffset", "leftShoulderAngle"));
	offsetQ.AddRotation(leftShoulder_Quat_);
	playerModels_->GetLeftShoulderTrans()->rotation_ = offsetQ;
	// 右肩を回転
	rightShoulder_Quat_ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, sin);
	RotationQuaternion(offsetQ, global_->GetValue<Vector3>("PlayerModelOffset", "rightShoulderAngle"));
	offsetQ.AddRotation(rightShoulder_Quat_);
	playerModels_->GetRightShoulderTrans()->rotation_ = offsetQ;
}

void PlayerAnimation::AttackCombo1()
{
	// アニメーションを更新
	int animationFrameNum = global_->GetValue<int>("AttackCombo1", "frameNum");
	for (int i = 1; i <= animationFrameNum; ++i) {
		if (combo1Frame_ >= static_cast<float>(i - 1) && combo1Frame_ < static_cast<float>(i)) {
			combo1Frame_ += 1.0f / global_->GetValue<float>("AttackCombo1", "frame" + std::to_string(i));
			AnimationUpdate(1, i);
		}
	}

	// 次のコンボに行ける時間
	nextCombo_ = false;
	if (combo1Frame_ > 2.0f && combo1Frame_ <= 3.0f) {
		nextCombo_ = true;
	}

	// 最後のフレームまで行ったらフラグを完了にする
	if (combo1Frame_ > static_cast<float>(animationFrameNum)) {
		combo1Completion_ = true;
	}

}

void PlayerAnimation::AttackCombo2()
{
	// アニメーションを更新
	int animationFrameNum = global_->GetValue<int>("AttackCombo2", "frameNum");
	for (int i = 1; i <= animationFrameNum; ++i) {
		if (combo2Frame_ >= static_cast<float>(i - 1) && combo2Frame_ < static_cast<float>(i)) {
			combo2Frame_ += 1.0f / global_->GetValue<float>("AttackCombo2", "frame" + std::to_string(i));
			AnimationUpdate(2, i);
		}
	}

	// 次のコンボに行ける時間
	nextCombo_ = false;
	if (combo2Frame_ > 2.0f && combo2Frame_ <= 3.0f) {
		nextCombo_ = true;
	}

	// 最後のフレームまで行ったらフラグを完了にする
	if (combo2Frame_ > static_cast<float>(animationFrameNum)) {
		combo2Completion_ = true;
	}
}

void PlayerAnimation::AttackCombo3()
{
	// アニメーションを更新
	int animationFrameNum = global_->GetValue<int>("AttackCombo3", "frameNum");
	for (int i = 1; i <= animationFrameNum; ++i) {
		if (combo3Frame_ >= static_cast<float>(i - 1) && combo3Frame_ < static_cast<float>(i)) {
			combo3Frame_ += 1.0f / global_->GetValue<float>("AttackCombo3", "frame" + std::to_string(i));
			AnimationUpdate(3, i);
		}
	}

	// 最後のフレームまで行ったらフラグを完了にする
	if (combo3Frame_ > static_cast<float>(animationFrameNum)) {
		combo3Completion_ = true;
	}
}
