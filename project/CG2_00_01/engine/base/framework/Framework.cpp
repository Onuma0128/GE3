#include "Framework.h"

void Framework::Initialize()
{
	winApp_ = std::make_unique<WinApp>();
	winApp_->Initialize();

	directXEngine_ = std::make_unique<DirectXEngine>();
	directXEngine_->Initialize(winApp_.get());

	Input::GetInstance()->Initialize(winApp_.get());

	imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(directXEngine_.get(), winApp_.get());
}

void Framework::Finalize()
{
	delete sceneFactory_;

	Input::GetInstance()->Finalize();

	SceneManager::GetInstance()->Finalize();
}

void Framework::Update()
{
	//Windowにメッセージが来てたら最優先で処理させる
	if (winApp_->ProcessMessage()) {
		endRequst_ = true;
	}
	else {
		// 入力の更新
		Input::GetInstance()->Update();

		// シーンの更新
		SceneManager::GetInstance()->Update();
	}
}

void Framework::Run()
{
	Initialize();

	//ウィンドウの×ボタンが押されるまでループ
	while (true) {
		// 更新
		Update();
		// 呼び出されたらループ終了
		if (IsEndRequst()) {
			break;
		}
		// 描画
		Draw();
	}
	// 終了
	Finalize();
}
