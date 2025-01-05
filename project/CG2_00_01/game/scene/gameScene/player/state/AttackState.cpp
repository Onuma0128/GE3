#include "AttackState.h"

#include "AudioManager.h"

#include "gameScene/player/Player.h"
#include "gameScene/animation/PlayerAnimation.h"

#include "gameScene/player/state/MoveState.h"
#include "gameScene/player/state/DashAttackState.h"

AttackState::AttackState(Player* player, PlayerAnimation* playerAnimation) : BaseState(player, playerAnimation) {}

void AttackState::Initialize()
{
	nowCombo_ = AttackCombo::Combo1;

	// 剣のワールド座標を取得
	Vector3 world = Vector3{ global_->GetValue<Vector3>("PlayerSwordParticle", "position") }.Transform(playerAnimation_->GetPlayerModels()->GetSwordTrans()->matWorld_);
	Vector3 acceleration = Vector3{ global_->GetValue<Vector3>("PlayerSwordParticle", "acceleration") }.Transform(Quaternion::MakeRotateMatrix(player_->GetTransform()->rotation_));
	player_->GetSwordEmitter()->SetIsCreate(false);
	player_->GetSwordEmitter()->SetPosition(world);
	player_->GetSwordEmitter()->SetAcceleration(acceleration);
	if (playerAnimation_->GetCombo2Frame() >= 2.0f) {
		nowCombo_ = AttackCombo::Combo3;
	}
	playerAnimation_->Reset();
}

void AttackState::Update()
{
	player_->GetSwordEmitter()->SetIsCreate(false);
	// 剣のワールド座標を取得
	Vector3 world = Vector3{ global_->GetValue<Vector3>("PlayerSwordParticle", "position") }.Transform(playerAnimation_->GetPlayerModels()->GetSwordTrans()->matWorld_);

	switch (nowCombo_)
	{
	case AttackState::AttackCombo::Combo1:
		// 2コンボ目のアニメーションを更新
		playerAnimation_->AttackCombo1();
		player_->GetSwordEmitter()->SetIsCreate(true);
		player_->GetSwordEmitter()->SetPosition(world);
		player_->SetIsAttack(true);

		if (playerAnimation_->GetCombo1Frame() < 1.0f || playerAnimation_->GetCombo1Frame() > 1.7f) {
			player_->GetSwordEmitter()->SetIsCreate(false);
			player_->SetIsAttack(false);
		}
		// 効果音
		if (playerAnimation_->GetCombo1Frame() >= 1.0f &&
			playerAnimation_->GetCombo1Frame() < 1.0f + (1.0f / global_->GetValue<float>("AttackCombo1", "frame2") * 2.0f)) {
			player_->GetAudio()->SoundPlayWave("Combo1.wav", 0.25f);
		}

		// 条件を達成したら次のコンボに移動
		if (input_->PushGamepadButton(XINPUT_GAMEPAD_A) && playerAnimation_->GetNextCombo()) {
			//playerAnimation_->Reset();
			Vector3 acceleration = Vector3{ global_->GetValue<Vector3>("PlayerSwordParticle", "acceleration2") }.Transform(Quaternion::MakeRotateMatrix(player_->GetTransform()->rotation_));
			player_->GetSwordEmitter()->SetAcceleration(acceleration);
			nowCombo_ = AttackCombo::Combo2;
			return;
		}

		// 攻撃が終了したらステートを変更
		if (playerAnimation_->GetCombo1Completion()) {
			playerAnimation_->Reset();
			player_->ChengeState(std::make_unique<MoveState>(player_, playerAnimation_));
			return;
		}

		break;
	case AttackState::AttackCombo::Combo2:
		// 2コンボ目のアニメーションを更新
		playerAnimation_->AttackCombo2();
		player_->GetSwordEmitter()->SetIsCreate(true);
		player_->GetSwordEmitter()->SetPosition(world);
		player_->SetIsAttack(true);

		if (playerAnimation_->GetCombo2Frame() < 1.0f || playerAnimation_->GetCombo2Frame() > 1.6f) {
			player_->GetSwordEmitter()->SetIsCreate(false);
			player_->SetIsAttack(false);
		}
		// 効果音
		if (playerAnimation_->GetCombo2Frame() >= 1.0f &&
			playerAnimation_->GetCombo2Frame() < 1.0f + (1.0f / global_->GetValue<float>("AttackCombo2", "frame2") * 2.0f)) {
			player_->GetAudio()->SoundPlayWave("Combo2.wav", 0.25f);
		}

		// 条件を達成したら次のコンボに移動
		if (input_->PushGamepadButton(XINPUT_GAMEPAD_A) && playerAnimation_->GetNextCombo()) {
			//playerAnimation_->Reset();
			Vector3 acceleration = Vector3{ global_->GetValue<Vector3>("PlayerSwordParticle", "acceleration3") }.Transform(Quaternion::MakeRotateMatrix(player_->GetTransform()->rotation_));
			player_->GetSwordEmitter()->SetAcceleration(acceleration);
			nowCombo_ = AttackCombo::Combo3;
			return;
		}
		// 2コンボ目から突撃攻撃に派生
		if (input_->PushGamepadButton(XINPUT_GAMEPAD_B) && playerAnimation_->GetNextCombo()) {
			//playerAnimation_->Reset();
			Vector3 acceleration = Vector3{ global_->GetValue<Vector3>("PlayerSwordParticle", "acceleration4") }.Transform(Quaternion::MakeRotateMatrix(player_->GetTransform()->rotation_));
			player_->GetSwordEmitter()->SetAcceleration(acceleration);
			nowCombo_ = AttackCombo::Combo3;
			player_->ChengeState(std::make_unique<DashAttackState>(player_, playerAnimation_));
			return;
		}

		// 攻撃が終了したらステートを変更
		if (playerAnimation_->GetCombo2Completion()) {
			playerAnimation_->Reset();
			player_->ChengeState(std::make_unique<MoveState>(player_, playerAnimation_));
			return;
		}

		break;
	case AttackState::AttackCombo::Combo3:
		// 3コンボ目のアニメーションを更新
		playerAnimation_->AttackCombo3();
		player_->GetSwordEmitter()->SetIsCreate(true);
		player_->GetSwordEmitter()->SetPosition(world);
		player_->SetIsAttack(true);

		if (playerAnimation_->GetCombo3Frame() < 1.9f || playerAnimation_->GetCombo3Frame() > 2.5f) {
			player_->GetSwordEmitter()->SetIsCreate(false);
			player_->SetIsAttack(false);
		}
		// 効果音
		if (playerAnimation_->GetCombo3Frame() >= 2.0f &&
			playerAnimation_->GetCombo3Frame() < 2.0f + (1.0f / global_->GetValue<float>("AttackCombo3", "frame3") * 3.0f)) {
			player_->GetAudio()->SoundPlayWave("Combo3.wav", 0.25f);
		}
		if (playerAnimation_->GetCombo3Frame() >= 2.5f &&
			playerAnimation_->GetCombo3Frame() < 2.5f + (1.0f / global_->GetValue<float>("AttackCombo3", "frame3") * 2.0f)) {
			player_->SetIsShake(true);
		}

		// 攻撃が終了したらステートを変更
		if (playerAnimation_->GetCombo3Completion()) {
			playerAnimation_->Reset();
			player_->ChengeState(std::make_unique<MoveState>(player_, playerAnimation_));
			return;
		}

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
