#include "Player.h"

#include "TextureManager.h"

void Player::Init()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("uvChecker.png");
	sprite_->SetPosition({ 64,64 });
	sprite_->SetSize({ 128,128 });
	sprite_->SetAnchorPoint({ 0.5f,0.5f });
}

void Player::Update()
{
	sprite_->Update();
}

void Player::Draw()
{
	sprite_->Draw();
}

void Player::MoveRight()
{
	position_ = sprite_->GetPosition();

	position_.x += speed_;

	sprite_->SetPosition(position_);
}

void Player::MoveLeft()
{
	position_ = sprite_->GetPosition();

	position_.x -= speed_;

	sprite_->SetPosition(position_);
}
