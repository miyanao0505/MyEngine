#pragma once
#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>
#include <Windows.h>
#include <wrl.h>
#include <array>
#include "WindowsAPI.h"
#include "InputTypes.h"

/// <summary>
/// 入力管理クラス(キーボード・マウス対応)
/// </summary>
class Input
{
private:
#pragma region 定数
	static constexpr size_t kKeyCount = 256;
	static constexpr size_t kMouseCount = 5;	// 左・右・中・サイド1・サイド2
#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>Input クラスの唯一のインスタンス</returns>
	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApi">WindowsAPIのポインタ</param>
	void Initialize(WindowsAPI* winApi);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:	// メンバ関数
#pragma region キーボード
	/// <summary>
	/// キーボードの初期化
	/// </summary>
	void KeyboardInitialize();

	/// <summary>
	/// キーボードの更新
	/// </summary>
	void KeyboardUpdate();
#pragma endregion

#pragma region マウス
	/// <summary>
	/// マウスの初期化
	/// </summary>
	void MouseInitialize();

	/// <summary>
	/// マウスの更新
	/// </summary>
	void MouseUpdate();

	/// <summary>
	/// マウス情報の更新
	/// </summary>
	void MouseStateUpdate();

	/// <summary>
	/// マウス座標更新
	/// UIヒット判定で使用するためクライアント座標に変換
	/// </summary>
	void MousePosUpdate();

	/// <summary>
	/// マウス移動量更新
	/// </summary>
	void MouseMoveUpdate();

	/// <summary>
	/// マウスホイールの更新
	/// </summary>
	void MouseWheelUpdate();
#pragma endregion

public:	// getter
#pragma region キーボード関係
	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号(DIK_0 等)</param>
	/// <returns>指定キーが押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号(DIK_0 等)</param>
	/// <returns>指定キーがトリガー状態か</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// キーのリリースをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号(DIK_0 等)</param>
	/// <returns>指定キーがリリース状態か</returns>
	bool ReleaseKey(BYTE keyNumber);
#pragma endregion

#pragma region マウス関係
	/// <summary>
	/// マウスボタンの押下をチェック
	/// </summary>
	/// <param name="button">マウスボタン番号</param>
	/// <returns>指定マウスボタンが押されているか</returns>
	bool PushMouse(MouseButton button);

	/// <summary>
	/// マウスボタンのトリガーをチェック
	/// </summary>
	/// <param name="button">マウスボタン番号</param>
	/// <returns>指定マウスボタンがトリガー状態か</returns>
	bool TriggerMouse(MouseButton button);

	/// <summary>
	/// マウスボタンのリリースをチェック
	/// </summary>
	/// <param name="button">マウスボタン番号</param>
	/// <returns>指定マウスボタンがリリース状態か</returns>
	bool ReleaseMouse(MouseButton button);

	/// <summary>
	/// マウスカーソルの座標取得
	/// </summary>
	/// <returns>マウスカーソルの座標</returns>
	POINT GetMousePosition();

	/// <summary>
	/// マウスカーソルの移動距離取得
	/// </summary>
	/// <returns>マウスカーソルの移動距離</returns>
	POINT GetMouseMove();

	/// <summary>
	/// マウスホイールの回転量取得
	/// </summary>
	/// <returns>マウスホイールの回転量</returns>
	int GetWheelDelta();
#pragma endregion

public:	// setter

private:	// シングルトンインスタンス
	static Input* sInstance;

	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

private: // メンバ変数
#pragma region キーボード
	// キーボードのデバイス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_;

	// 前回の全キーの状態
	std::array<BYTE, kKeyCount> keyPre_;
	// 全キーの状態
	std::array<BYTE, kKeyCount> key_;
#pragma endregion

#pragma region マウス
	// マウスのデバイス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouseDevice_;
	HRESULT mouseHr;

	// マウス状態(DirectInput)
	DIMOUSESTATE2 mouseState_;
	DIMOUSESTATE2 mouseStatePre_;

	// 前回のマウスボタンの状態
	std::array <BYTE, kMouseCount> mouseButtonsPre_;
	// マウスボタンの状態
	std::array <BYTE, kMouseCount> mouseButtons_;

	// 前回のマウスカーソルの座標
	POINT mousePosPre_;
	// マウスカーソルの座標
	POINT mousePos_;
	// マウスの移動量
	POINT mouseMove_;

	// ホイールの回転量
	int wheelDelta_ = 0;
#pragma endregion

	// DirectInputのインスタンス
	Microsoft::WRL::ComPtr<IDirectInput8> directInput_;
	// WindowsAPI
	WindowsAPI* winApi_ = nullptr;
};