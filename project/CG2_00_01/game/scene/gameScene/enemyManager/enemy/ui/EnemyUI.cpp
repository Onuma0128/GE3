#include "EnemyUI.h"

#include "Matrix4x4.h"
#include "imgui.h"

#include "gameScene/gameCamera/GameCamera.h"
#include "gameScene/enemyManager/enemy/Enemy.h"
#include "gameScene/player/Player.h"

void EnemyUI::Init()
{
	GlobalInit();

	hpSprite_ = std::make_unique<Sprite>();
	hpSprite_->Initialize("white1x1.png");
	hpSprite_->SetAnchorPoint({ 0.5f,0.5f });
}

void EnemyUI::GlobalInit()
{
}

void EnemyUI::Update()
{
	Matrix4x4 viewProjection = enemy_->GetGameCamera()->GetCamera()->GetViewProjectionMatrix();
	Matrix4x4 viewPort = enemy_->GetGameCamera()->GetCamera()->GetViewportMatrix();
	Vector3 enemyWorldPosition = enemy_->GetTransform()->GetWorldPosition();
	Vector3 playerWorldPosition = enemy_->GetPlayer()->GetTransform()->GetWorldPosition();

	Matrix4x4 screenMatrix = viewProjection * viewPort;
	Vector3 hpScreenPosition = enemyWorldPosition.Transform(screenMatrix) - Vector3{ 0,720,0 };
	Vector3 playerScreenPosition = playerWorldPosition.Transform(screenMatrix) - Vector3{ 0,720,0 };

	float lenght = (enemy_->GetTransform()->translation_ - enemy_->GetGameCamera()->GetCamera()->GetTranslate()).Length();
	float scale = 1.0f / (lenght * 0.05f); // 適宜調整
	scale = std::clamp(scale, 0.5f, 2.0f); // スケールの最小値・最大値を制限
	float baseSize = global_->GetValue<float>("Enemy", "hpSpriteSize");
	float hpSize_x = static_cast<float>(enemy_->GetHP() * baseSize * scale);
	hpSprite_->SetSize({ hpSize_x, baseSize / 1.5f * scale });
	Vector2 offset = { global_->GetValue<Vector3>("Enemy", "hpSpriteOffset").x,global_->GetValue<Vector3>("Enemy", "hpSpriteOffset").y };
	hpSprite_->SetPosition(Vector2{ hpScreenPosition.x + offset.x,(hpScreenPosition.y * -1.0f) + offset.y });
	float alpha = (50.0f - lenght) / 10.0f;
	alpha = std::clamp(alpha, 0.0f, 1.0f);

	if (alpha == 1.0f) {
		playerScreenPosition.y *= -1.0f;
		playerScreenPosition.y -= global_->GetValue<float>("Enemy", "playerSpriteY");
		hpScreenPosition.y *= -1.0f;
		Vector3 spriteLenght = playerScreenPosition - hpScreenPosition;
		lenght = std::sqrt(spriteLenght.x * spriteLenght.x + spriteLenght.y * spriteLenght.y);
		alpha = (lenght - 30.0f) / 100.0f;
		if (lenght <= 30.0f) { alpha = 0.0f; }
		alpha = std::clamp(alpha, 0.0f, 1.0f);
	}

	hpSprite_->SetColor(Vector4{ 1.0f,0.0f,0.0f,alpha });
	hpSprite_->Update();
}

void EnemyUI::Draw()
{
	float lenght = (enemy_->GetTransform()->translation_ - enemy_->GetGameCamera()->GetCamera()->GetTranslate()).Length();
	if (lenght <= 50) {
		hpSprite_->Draw();
	}
}
