#pragma once

#include <memory>

#include "Input.h"

#include "titleScene/command/Command.h"

class InputHandler
{
public:

	ICommand* HandleInput();
	
	// 左入力処理
	void AssignMoveLeftCommand2PressKeyA();
	// 右入力処理
	void AssignMoveRightCommand2PressKeyD();

private:

	std::unique_ptr<ICommand> pressKeyA_;
	std::unique_ptr<ICommand> pressKeyD_;

	Input* input_ = Input::GetInstance();

};

