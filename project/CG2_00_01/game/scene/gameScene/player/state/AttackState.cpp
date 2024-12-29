#include "AttackState.h"

#include "gameScene/player/Player.h"
#include "gameScene/animation/PlayerAnimation.h"

#include "gameScene/player/state/MoveState.h"

AttackState::AttackState(Player* player, PlayerAnimation* playerAnimation) : BaseState(player, playerAnimation) {}

void AttackState::Initialize()
{
	nowCombo_ = AttackCombo::Combo1;
}

void AttackState::Update()
{
	switch (nowCombo_)
	{
	case AttackState::AttackCombo::Combo1:
		playerAnimation_->AttackCombo1();

		// 条件を達成したら次のコンボに移動
		// nowCombo_ = AttackCombo::Combo2;
		if (playerAnimation_->GetCombo1Completion()) {
			playerAnimation_->Reset();
			player_->ChengeState(std::make_unique<MoveState>(player_, playerAnimation_));
			return;
		}

		break;
	case AttackState::AttackCombo::Combo2:
		playerAnimation_->AttackCombo2();

		// 条件を達成したら次のコンボに移動
		// nowCombo_ = AttackCombo::Combo3;

		break;
	case AttackState::AttackCombo::Combo3:
		playerAnimation_->AttackCombo3();

		// 条件を達成したらコンボ攻撃を終了
		// player_->ChengeState(std::make_unique<MoveState>(player_, playerAnimation_));

		break;
	default:
		break;
	}

	playerAnimation_->Update();
}

void AttackState::Draw()
{
	playerAnimation_->Draw();
}

void AttackState::Finalize()
{
}
