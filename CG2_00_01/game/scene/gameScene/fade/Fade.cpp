#include "Fade.h"

#include "SceneManager.h"

void Fade::Initialize()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("fade.png");
	sprite_->SetPosition({ 640,-360 });
	sprite_->SetAnchorPoint({ 0.5f,0.5f });
	sprite_->SetSize({ 1280,720 });

	time_ = 0.0f;
}

void Fade::Update()
{
	time_ += 0.01f;
	if (time_ >= 1.0f) {
		time_ = 1.0f;
		SceneManager::GetInstance()->ChangeScene("Title");
	}
	Vector2 position{
		.x = 640,
		.y = -360 + time_ * 720
	};
	sprite_->SetPosition(position);
	sprite_->Update();
}

void Fade::Draw()
{
	sprite_->Draw();
}
