#include "DashAttackState.h"

#include "gameScene/player/Player.h"
#include "gameScene/animation/PlayerAnimation.h"

#include "gameScene/player/state/MoveState.h"
#include "gameScene/player/state/AttackState.h"

DashAttackState::DashAttackState(Player* player, PlayerAnimation* playerAnimation) :BaseState(player, playerAnimation) {}

void DashAttackState::Initialize()
{
	// プレイヤーのz方向ベクトル
	Vector3 localDirection = Vector3::ExprUnitZ;
	Vector3 globalDirection = Quaternion::RotateVector(localDirection, player_->GetTransform()->rotation_);
	velocity_ = globalDirection.Normalize();
}

void DashAttackState::Update()
{
	playerAnimation_->DashAttack();
	player_->GetMoveEmitter()->SetIsCreate(false);
	// 剣のトレイル座標
	Matrix4x4 worldMatrix = playerAnimation_->GetPlayerModels()->GetSwordTrans()->matWorld_;
	Vector3 pos1 = Vector3{ global_->GetValue<Vector3>("PlayerTrailEffect", "position0") }.Transform(worldMatrix);
	Vector3 pos2 = Vector3{ global_->GetValue<Vector3>("PlayerTrailEffect", "position1") }.Transform(worldMatrix);

	player_->SetIsAttack(true);

	// 効果音
	if (playerAnimation_->GetDashFrame() >= 1.0f &&
		playerAnimation_->GetDashFrame() < 1.0f + (1.0f / global_->GetValue<float>("DashAttack", "frame2") * 2.0f)) {
		player_->GetAudio()->SoundPlayWave("Dash.wav", 0.25f);
	}

	if (playerAnimation_->GetDashFrame() > 1.0f && playerAnimation_->GetDashFrame() < 2.0f) {
		player_->GetMoveEmitter()->SetIsCreate(true);
		Vector3 translation = player_->GetTransform()->translation_;
		translation += velocity_ * global_->GetValue<float>("Player", "dashPow") * (2.0f - playerAnimation_->GetDashFrame());
		player_->GetTransform()->translation_ = translation;
		CreateSwordEffect(pos1, pos2);
	}
	else {
		player_->SetIsAttack(false);
	}

	// コンボアタックから自動で3コンボ目に遷移
	if (playerAnimation_->GetCombo2Frame() >= 2.0f && playerAnimation_->GetNextCombo()) {
		player_->ChengeState(std::make_unique<AttackState>(player_, playerAnimation_));
		return;
	}

	// 攻撃が終了したらステートを変更
	if (playerAnimation_->GetDashCompletion()) {
		playerAnimation_->Reset();
		player_->ChengeState(std::make_unique<MoveState>(player_, playerAnimation_));
		return;
	}

	// エミッターの処理
	player_->GetMoveEmitter()->SetPosition(player_->GetTransform()->translation_);
	Vector3 acceleration = player_->GetVelocity() * global_->GetValue<float>("Player", "dustAcceleration");
	acceleration.y = global_->GetValue<float>("Player", "dustAccelerationY");
	player_->GetMoveEmitter()->SetAcceleration(acceleration);

	playerAnimation_->Update();
}

void DashAttackState::Draw()
{
	playerAnimation_->Draw();
}

void DashAttackState::Finalize()
{
}

void DashAttackState::CreateSwordEffect(const Vector3& pos1, const Vector3& pos2)
{
	trailPositions_.push_back(pos1);
	trailPositions_.push_back(pos2);
	if (trailPositions_.size() >= 4) {
		for (auto& trail : player_->GetPlayerEffect()->GetTrailEffects()) {
			if (trail.alpha_ <= 0.0f) {
				trail.effect_->SetPosition(trailPositions_);
				trail.alpha_ = 1.0f;
				trailPositions_.erase(trailPositions_.begin());
				trailPositions_.erase(trailPositions_.begin());
				break;
			}
		}
	}
}
