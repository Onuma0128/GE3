#pragma once
#include <memory>

#include "WorldTransform.h"
#include "GlobalVariables.h"

#include "PlayerModels.h"

class Player;

class PlayerAnimation
{
public:

	void Init();

	void Update();

	void Draw();

	void GlovalInit();

	void Debug_ImGui();

	void Reset();

	// 通常時のアニメーション
	void NormalAnimation();

	// 攻撃時のアニメーション
	void AttackCombo1();
	void AttackCombo2();
	void AttackCombo3();
	
	const float GetCombo1Frame() { return combo1Frame_; }
	const float GetCombo2Frame() { return combo2Frame_; }
	const float GetCombo3Frame() { return combo3Frame_; }

	// 次のコンボに遷移用フラグ
	const bool GetNextCombo() { return nextCombo_; }

	// 攻撃時のアニメーション完了フラグ
	const bool GetCombo1Completion() { return combo1Completion_; }
	const bool GetCombo2Completion() { return combo2Completion_; }
	const bool GetCombo3Completion() { return combo3Completion_; }

	// プレイヤーのポインタを取得
	void SetPlayer(Player* player) { player_ = player; }

	PlayerModels* GetPlayerModels()const { return playerModels_.get(); }

private:

	// 座標計算
	float LerpShortAngleExtended(float a, float b, float t);
	void Vector3LerpShortPosition(Vector3& v1, const Vector3& v2, float t);
	// 回転計算
	void RotationQuaternion(Quaternion& q, const Vector3& v);

	// 新しいフレームを追加
	void AddGlobalVariables(int comboNum,int frameNum);

	// フレーム間のアニメーションを更新
	void AnimationUpdate(int comboNum, int frameNum);

private:

	Player* player_ = nullptr;
	GlobalVariables* global_ = GlobalVariables::GetInstance();

	std::unique_ptr<PlayerModels> playerModels_ = nullptr;

	// 通常時用のフレーム
	float normalFrame_ = 0.0f;
	Quaternion leftShoulder_Quat_ = Quaternion::IdentityQuaternion();
	Quaternion rightShoulder_Quat_ = Quaternion::IdentityQuaternion();
	
	// 攻撃1コンボのフレーム
	float combo1Frame_ = 0.0f;
	// 攻撃2コンボのフレーム
	float combo2Frame_ = 0.0f;
	// 攻撃3コンボのフレーム
	float combo3Frame_ = 0.0f;

	// 次のコンボに遷移用フラグ
	bool nextCombo_ = false;

	// コンボの終了を教えるフラグ
	bool combo1Completion_ = false;
	bool combo2Completion_ = false;
	bool combo3Completion_ = false;

};