#include "MyGame.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	MyGame myGame_;
	// 初期化
	myGame_.Initialize();

	//ウィンドウの×ボタンが押されるまでループ
	while (true) {
		// 更新
		myGame_.Update();
		// 呼び出されたらループ終了
		if (myGame_.isEndRequst()) {
			break;
		}
		// 描画
		myGame_.Draw();	
	}
	// 終了
	myGame_.Finalize();

	return 0;
}