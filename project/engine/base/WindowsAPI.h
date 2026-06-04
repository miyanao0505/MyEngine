#pragma once
#include <Windows.h>
#include <cstdint>

/// <summary>
/// アプリケーションウィンドウの生成と管理を行うクラス。
/// </summary>
class WindowsAPI
{
public:	// 静的メンバ関数
	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="msg">メッセージID</param>
	/// <param name="wparam">メッセージの追加情報1</param>
	/// <param name="lparam">メッセージの追加情報2</param>
	/// <returns>メッセージ処理の結果</returns>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="title">ウィンドウタイトル</param>
	void Initialize(const wchar_t* title);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns>終了要求が来たらfalseを返す</returns>
	bool ProcessMessage();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

public:	// getter
	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns>HWND</returns>
	HWND GetHwnd() const { return hwnd; }

	/// <summary>
	/// HINSTANCEの取得
	/// </summary>
	/// <returns>HINSTANCE</returns>
	HINSTANCE GetHInstance() const { return windowClass_.hInstance; }

private:
	// ウィンドウハンドル
	HWND hwnd = nullptr;
	// ウィンドウクラスの設定
	WNDCLASS windowClass_{};

#pragma region 定数
	public:
	// クライアント領域のサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

private:
	static constexpr UINT kTimerResolutionMs = 1; // タイマーの分解能[ms]
	static constexpr wchar_t kWindowClassName[] = L"MyEngineWindowClass";
#pragma endregion
};