#pragma once
#include <Windows.h>
#include <cstdint>

// WindowsAPI
class WindowsAPI
{
public:	// 静的メンバ関数
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:	// メンバ関数
	// 初期化
	void Initialize(const wchar_t* title);
	// 更新
	void Update();

	// メッセージの処理
	bool ProcessMessage();
	// 終了
	void Finalize();

public:	// getter
	HWND GetHwnd() const { return hwnd; }
	HINSTANCE GetHInstance() const { return wc.hInstance; }

public:	// 定数
	// クライアント領域のサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

private:
	// ウィンドウハンドル
	HWND hwnd = nullptr;
	// ウィンドウクラスの設定
	WNDCLASS wc{};
};
