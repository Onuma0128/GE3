#include "FadeScene.h"

#include <algorithm>

#include "SceneManager.h"

void FadeScene::Init(float setAlpha)
{
	fade_.sprite_ = std::make_unique<Sprite>();
	fade_.sprite_->Initialize("white1x1.png");
	fade_.sprite_->SetSize(Vector2{ 1280.0f,720.0f });
	fade_.sprite_->SetColor(Vector4{ 1.0f,1.0f,1.0f,setAlpha });
	fade_.fadeFrame_ = 0.0f;
	fade_.alpha_ = 1.0f;
}

void FadeScene::Update()
{

}

void FadeScene::Draw()
{
	fade_.sprite_->Draw();
}

void FadeScene::FadeIn(const std::string& newSceneName, const Vector3& color, float fadeFrame)
{
	fade_.fadeFrame_ += 1.0f / fadeFrame;
	fade_.alpha_ = fade_.fadeFrame_;
	fade_.sprite_->SetColor(Vector4{ color.x,color.y,color.z,fade_.alpha_ });
	fade_.sprite_->Update();

	if (fade_.fadeFrame_ >= 1.0f) {
		fade_.fadeFrame_ = 0.0f;
		SceneManager::GetInstance()->ChangeScene(newSceneName);
	}
}
