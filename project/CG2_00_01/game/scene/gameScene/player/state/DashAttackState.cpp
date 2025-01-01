#include "DashAttackState.h"

#include "gameScene/player/Player.h"
#include "gameScene/animation/PlayerAnimation.h"

#include "gameScene/player/state/MoveState.h"
#include "gameScene/player/state/AttackState.h"

DashAttackState::DashAttackState(Player* player, PlayerAnimation* playerAnimation) :BaseState(player, playerAnimation) {}

void DashAttackState::Initialize()
{
	// 剣のワールド座標を取得
	Vector3 world = Vector3{ global_->GetValue<Vector3>("PlayerSwordParticle", "position") }.Transform(playerAnimation_->GetPlayerModels()->GetSwordTrans()->matWorld_);
	Vector3 acceleration = Vector3{ global_->GetValue<Vector3>("PlayerSwordParticle", "acceleration4") }.Transform(Quaternion::MakeRotateMatrix(player_->GetTransform()->rotation_));
	player_->GetSwordEmitter()->SetIsCreate(false);
	player_->GetSwordEmitter()->SetPosition(world);
	player_->GetSwordEmitter()->SetAcceleration(acceleration);
}

void DashAttackState::Update()
{
	playerAnimation_->DashAttack();
	player_->GetSwordEmitter()->SetIsCreate(false);
	// 剣のワールド座標を取得
	Vector3 world = Vector3{ global_->GetValue<Vector3>("PlayerSwordParticle", "position") }.Transform(playerAnimation_->GetPlayerModels()->GetSwordTrans()->matWorld_);
	
	player_->GetSwordEmitter()->SetIsCreate(true);
	player_->GetSwordEmitter()->SetPosition(world);
	player_->SetIsAttack(true);

	if (playerAnimation_->GetDashFrame() < 1.0f || playerAnimation_->GetDashFrame() > 1.6f) {
		player_->GetSwordEmitter()->SetIsCreate(false);
	}
	if (playerAnimation_->GetDashFrame() > 1.0f && playerAnimation_->GetDashFrame() < 2.0f) {
		Vector3 translation = player_->GetTransform()->translation_;
		translation += player_->GetVelocity() * global_->GetValue<float>("Player", "dashPow") * (2.0f - playerAnimation_->GetDashFrame());
		player_->GetTransform()->translation_ = translation;
	}
	else {
		player_->SetIsAttack(false);
	}


	// コンボアタックに遷移
	if (input_->PushGamepadButton(XINPUT_GAMEPAD_A) && playerAnimation_->GetNextCombo()) {
		player_->ChengeState(std::make_unique<AttackState>(player_, playerAnimation_));
		return;
	}

	// 攻撃が終了したらステートを変更
	if (playerAnimation_->GetDashCompletion()) {
		playerAnimation_->Reset();
		player_->ChengeState(std::make_unique<MoveState>(player_, playerAnimation_));
		return;
	}

	playerAnimation_->Update();
}

void DashAttackState::Draw()
{
	playerAnimation_->Draw();
}

void DashAttackState::Finalize()
{
}
