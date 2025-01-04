#include "PlayerUI.h"

#include "gameScene/player/Player.h"

void PlayerUI::Init()
{
	GlobalInit();

	A_Button_ = std::make_unique<Sprite>();
	A_Button_->Initialize("ui/A_button.png");
	A_Button_->SetAnchorPoint(Vector2{ 0.5f,0.5f });

	B_Button_ = std::make_unique<Sprite>();
	B_Button_->Initialize("ui/B_button.png");
	B_Button_->SetAnchorPoint(Vector2{ 0.5f,0.5f });

	attack1Sprite_ = std::make_unique<Sprite>();
	attack1Sprite_->Initialize("ui/Attack1.png");
	attack1Sprite_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	attack2Sprite_ = std::make_unique<Sprite>();
	attack2Sprite_->Initialize("ui/Attack2.png");
	attack2Sprite_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	attack3Sprite_ = std::make_unique<Sprite>();
	attack3Sprite_->Initialize("ui/Attack3.png");
	attack3Sprite_->SetAnchorPoint(Vector2{ 0.5f,0.5f });
	dashSprite_ = std::make_unique<Sprite>();
	dashSprite_->Initialize("ui/Dash.png");
	dashSprite_->SetAnchorPoint(Vector2{ 0.5f,0.5f });

	for (int i = 0; i < 5; ++i) {
		std::unique_ptr<Sprite> hp = std::make_unique<Sprite>();
		hp->Initialize("player/texture/heart.png");
		hp->SetAnchorPoint(Vector2{ 0.5f,0.5f });
		hp->SetSize(Vector2{ 50.0f,50.0f });
		hp->SetPosition(Vector2{ static_cast<float>(i) * 58.0f + 41.0f,679.0f });
		hpSprites_.push_back(std::move(hp));
	}

	SpriteOffset();
}

void PlayerUI::GlobalInit()
{
	global_->AddValue<Vector3>("PlayerUI", "A_ButtonPos", Vector3{});
	global_->AddValue<Vector3>("PlayerUI", "B_ButtonPos", Vector3{});

	global_->AddValue<Vector3>("PlayerUI", "A_ButtonColor", Vector3{ 1,1,1 });
	global_->AddValue<Vector3>("PlayerUI", "B_ButtonColor", Vector3{ 1,1,1 });


	global_->AddValue<Vector3>("PlayerUI", "attack1Pos", Vector3{});
	global_->AddValue<Vector3>("PlayerUI", "attack2Pos", Vector3{});
	global_->AddValue<Vector3>("PlayerUI", "attack3Pos", Vector3{});
	global_->AddValue<Vector3>("PlayerUI", "DashPos", Vector3{});

}

void PlayerUI::Update()
{
	SpriteOffset();

	// ボタンの色指定
	Vector3 A_ButtonColor = global_->GetValue<Vector3>("PlayerUI", "A_ButtonColor");
	Vector3 B_ButtonColor = global_->GetValue<Vector3>("PlayerUI", "B_ButtonColor");
	if (input_->PushGamepadButton(XINPUT_GAMEPAD_A)) {
		A_Button_->SetColor(Vector4{ A_ButtonColor.x,A_ButtonColor.y,A_ButtonColor.z,1.0f });
	}
	else {
		A_Button_->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
	}
	if (input_->PushGamepadButton(XINPUT_GAMEPAD_B)) {
		B_Button_->SetColor(Vector4{ B_ButtonColor.x,B_ButtonColor.y,B_ButtonColor.z,1.0f });
	}
	else {
		B_Button_->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
	}

	A_Button_->Update();
	B_Button_->Update();

	attack1Sprite_->Update();
	attack2Sprite_->Update();
	attack3Sprite_->Update();
	dashSprite_->Update();

	for (auto& hp : hpSprites_) {
		hp->Update();
	}
}

void PlayerUI::Draw()
{
	A_Button_->Draw();
	B_Button_->Draw();

	DrawAttackUI();

	for (auto& hp : hpSprites_) {
		hp->Draw();
	}
}

void PlayerUI::DrawAttackUI()
{
	if (player_->GetPlayerAnima()->GetCombo1Frame() == 0.0f && player_->GetPlayerAnima()->GetDashFrame() == 0.0f) {
		attack1Sprite_->Draw();
		dashSprite_->Draw();
	}
	else if (player_->GetPlayerAnima()->GetCombo2Frame() == 0.0f && player_->GetPlayerAnima()->GetCombo1Frame() != 0.0f) {
		attack2Sprite_->Draw();
	}
	else if (player_->GetPlayerAnima()->GetCombo3Frame() == 0.0f && player_->GetPlayerAnima()->GetCombo2Frame() != 0.0f) {
		if (player_->GetPlayerAnima()->GetDashFrame() == 0.0f) {
			dashSprite_->Draw();
		}
		attack3Sprite_->Draw();
	}
}

void PlayerUI::DeleteHP()
{
	hpSprites_.pop_back();
}

void PlayerUI::SpriteOffset()
{
	// 攻撃UIの座標指定
	Vector3 attack1Pos = global_->GetValue<Vector3>("PlayerUI", "attack1Pos");
	Vector3 attack2Pos = global_->GetValue<Vector3>("PlayerUI", "attack2Pos");
	Vector3 attack3Pos = global_->GetValue<Vector3>("PlayerUI", "attack3Pos");
	Vector3 dashPos = global_->GetValue<Vector3>("PlayerUI", "DashPos");
	attack1Sprite_->SetPosition(Vector2{ attack1Pos.x,attack1Pos.y });
	attack2Sprite_->SetPosition(Vector2{ attack2Pos.x,attack2Pos.y });
	attack3Sprite_->SetPosition(Vector2{ attack3Pos.x,attack3Pos.y });
	dashSprite_->SetPosition(Vector2{ dashPos.x,dashPos.y });

	// ボタンの座標指定
	Vector3 A_ButtonPos = global_->GetValue<Vector3>("PlayerUI", "A_ButtonPos");
	Vector3 B_ButtonPos = global_->GetValue<Vector3>("PlayerUI", "B_ButtonPos");
	A_Button_->SetPosition(Vector2{ A_ButtonPos.x,A_ButtonPos.y });
	B_Button_->SetPosition(Vector2{ B_ButtonPos.x,B_ButtonPos.y });
}
