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

	void Reset();

	// 通常時のアニメーション
	void NormalAnimation();

	// 攻撃時のアニメーション
	void AttackCombo1();
	void AttackCombo2();
	void AttackCombo3();

	// プレイヤーのポインタを取得
	void SetPlayer(Player* player) { player_ = player; }

private:

	Player* player_ = nullptr;
	GlobalVariables* global_ = GlobalVariables::GetInstance();

	std::unique_ptr<PlayerModels> playerModels_ = nullptr;

	// 通常時用のフレーム
	float normalFrame_ = 0.0f;

};