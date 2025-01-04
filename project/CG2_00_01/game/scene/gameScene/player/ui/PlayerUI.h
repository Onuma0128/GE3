#pragma once

#include <memory>
#include <list>

#include "Sprite.h"
#include "GlobalVariables.h"
#include "Input.h"

class Player;

class PlayerUI
{
public:

	void Init();

	void GlobalInit();

	void Update();

	void Draw();

	void DrawAttackUI();

	void SetPlayer(Player* player) { player_ = player; }

	void DeleteHP();


private:

	void SpriteOffset();

private:

	Player* player_ = nullptr;
	GlobalVariables* global_ = GlobalVariables::GetInstance();
	Input* input_ = Input::GetInstance();

	// ボタン表示
	std::unique_ptr<Sprite> A_Button_ = nullptr;
	std::unique_ptr<Sprite> B_Button_ = nullptr;
	// 攻撃表示
	std::unique_ptr<Sprite> attack1Sprite_ = nullptr;
	std::unique_ptr<Sprite> attack2Sprite_ = nullptr;
	std::unique_ptr<Sprite> attack3Sprite_ = nullptr;
	std::unique_ptr<Sprite> dashSprite_ = nullptr;
	// HP表示
	std::list<std::unique_ptr<Sprite>> hpSprites_;

};