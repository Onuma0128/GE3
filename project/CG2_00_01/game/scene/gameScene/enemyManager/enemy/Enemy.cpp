#include "Enemy.h"

#include "gameScene/player/Player.h"

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	transform_ = std::make_unique<WorldTransform>();
	model_ = std::make_unique<Object3d>();
	model_->Initialize("box.obj", transform_.get());
}

void Enemy::Update()
{
	model_->Update();
}

void Enemy::Draw()
{
	model_->Draw();
}
