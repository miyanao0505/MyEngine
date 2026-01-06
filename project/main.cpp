#include "MyGame.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// リークチェック
	D3DResourceLeakChecker leakCheck;
	// ゲームクラスの生成
	auto game = std::make_unique<MyGame>();

	// ゲームループ
	game->Run(L"進空戦");

	return 0;
}