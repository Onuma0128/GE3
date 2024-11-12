#include "TitleScene.h"
#include "Input.h"
#include "SpriteBase.h"
#include "Object3dBase.h"

#include "SceneManager.h"

#include "globalVariables/GlobalVariables.h"

void TitleScene::Initialize()
{
	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	sprite->Initialize("start.png");
	sprite->SetPosition({ 640,360 });
	sprite->SetSize({ 1280,720 });
	sprite->SetAnchorPoint({ 0.5f,0.5f });
	sprites_.push_back(std::move(sprite));

	obj_ = std::make_unique<Object3d>("teapot.obj");
}

void TitleScene::Finalize()
{
	
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

	obj_->Update();

	for (auto& sprite : sprites_) {
		sprite->Update();
	}
}

void TitleScene::Draw()
{
	// Modelの描画準備
	Object3dBase::GetInstance()->DrawBase();
	obj_->Draw();

	// Spriteの描画準備
	SpriteBase::GetInstance()->DrawBase();

	for (auto& sprite : sprites_) {
		sprite->Draw();
	}
}
