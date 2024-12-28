#include "PlayerAnimation.h"

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
	global_->AddValue<float>("NormalAnimation", "frame", 60.0f);




}

void PlayerAnimation::Reset()
{
	normalFrame_ = 0.0f;
}

void PlayerAnimation::NormalAnimation()
{
	normalFrame_ += 1.0f / global_->GetValue<float>("NormalAnimation", "frame");
	float sin = std::sin(normalFrame_) * 0.1f;
	playerModels_->GetBodyTrans()->translation_.y = global_->GetValue<Vector3>("PlayerModelOffset", "body").y + sin;
	Quaternion leftShoulder = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, sin);
	playerModels_->GetLeftShoulderTrans()->rotation_ = leftShoulder;
	Quaternion rightShoulder = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, sin);
	playerModels_->GetRightShoulderTrans()->rotation_ = rightShoulder;
}

void PlayerAnimation::AttackCombo1()
{
}

void PlayerAnimation::AttackCombo2()
{
}

void PlayerAnimation::AttackCombo3()
{
}
