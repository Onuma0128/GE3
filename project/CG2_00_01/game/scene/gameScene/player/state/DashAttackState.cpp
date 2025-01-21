#include "DashAttackState.h"

#include "gameScene/gameTimer/GameTimer.h"
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

	isCreateEffect_ = false;

	isPlayAudio_ = false;
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
	if (playerAnimation_->GetDashFrame() >= 1.0f &&!isPlayAudio_) {
		player_->GetAudio()->SoundPlayWave("Dash.wav", 0.25f);
		isPlayAudio_ = true;
	}

	if (playerAnimation_->GetDashFrame() > 1.0f && playerAnimation_->GetDashFrame() < 2.0f) {
		player_->GetMoveEmitter()->SetIsCreate(true);
		Vector3 translation = player_->GetTransform()->translation_;
		translation += velocity_ * global_->GetValue<float>("Player", "dashPow") * (2.0f - playerAnimation_->GetDashFrame()) * GameTimer::GetInstance()->GetDeltaTime();
		player_->GetTransform()->translation_ = translation;
		CreateSwordEffect(pos1, pos2);
		if (!isCreateEffect_) {
			CreateDashEffect();
			isCreateEffect_ = true;
		}
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
	player_->GetMoveEmitter()->SetAcceleration(acceleration * GameTimer::GetInstance()->GetDeltaTime());

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

void DashAttackState::CreateDashEffect()
{
	int num = global_->GetValue<int>("PlayerDashEffect", "count");
	int count = 0;

	for (auto& dash : player_->GetPlayerEffect()->GetDashEffects()) {
		if (dash.alpha_ == 0.0f) {
			Vector3 randomPosition;
			do {
				Vector3 random = global_->GetValue<Vector3>("PlayerDashEffect", "randomPosition");
				randomPosition = {
					static_cast<float>(rand() % static_cast<int>(random.x) - static_cast<int>(random.x - 1) / 2) * 0.1f,
					static_cast<float>(rand() % static_cast<int>(random.y) + static_cast<int>(random.y - 1)) * 0.1f,
					static_cast<float>(rand() % static_cast<int>(random.z) - static_cast<int>(random.z - 1)) * 0.1f,
				};
			} while (randomPosition.Length() > 3.5f || randomPosition.Length() < 2.5f);

			Transform transform = dash.effect_->GetTransform();
			// サイズ
			transform.scale = global_->GetValue<Vector3>("PlayerDashEffect", "scale");
			// 座標
			Matrix4x4 rotateMatirx = Quaternion::MakeRotateMatrix(player_->GetTransform()->rotation_);
			transform.translate = player_->GetTransform()->translation_ + randomPosition.Transform(rotateMatirx);
			// 回転
			Vector3 randomVelocity = {};
			int randomX = rand() % static_cast<int>(global_->GetValue<float>("PlayerDashEffect", "velocitySize") + 1);
			int randomY = rand() % static_cast<int>(global_->GetValue<float>("PlayerDashEffect", "velocitySize") + 1);
			if (randomPosition.x > 0) {
				if (randomPosition.y >= 1.5f) {
					randomVelocity = {
						static_cast<float>(randomX) * 0.1f,
						static_cast<float>(randomY) * 0.1f,
						-0.3f
					};
				}
				else {
					randomVelocity = {
						static_cast<float>(randomX) * 0.1f,
						static_cast<float>(randomY) * -0.1f,
						-0.3f
					};
				}
			}
			else {
				if (randomPosition.y >= 1.5f) {
					randomVelocity = {
						static_cast<float>(randomX) * -0.1f,
						static_cast<float>(randomY) * 0.1f,
						-0.3f
					};
				}
				else {
					randomVelocity = {
						static_cast<float>(randomX) * -0.1f,
						static_cast<float>(randomY) * -0.1f,
						-0.3f
					};
				}
			}

			Vector3 velocity = randomVelocity.Transform(rotateMatirx);
			transform.rotate.y = std::atan2(velocity.x, velocity.z);
			Matrix4x4 rotateMatrixY = Matrix4x4::RotateY(-transform.rotate.y);
			Vector3 velocityZ = Vector3{ velocity }.Transform(rotateMatrixY);
			transform.rotate.x = std::atan2(-velocityZ.y, velocityZ.z);
			dash.effect_->SetTransform(transform);
			dash.alpha_ = 1.0f;
			dash.velocity_ = velocity.Normalize();
			++count;
			if (count == num) {
				break;
			}
		}
	}
}
