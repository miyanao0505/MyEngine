#pragma once
#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>
#include <Windows.h>
#include <wrl.h>
#include <array>
#include "WindowsAPI.h"

/// <summary>
/// キーボード入力を管理するクラス。
/// </summary>
class Input
{
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

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号(DIK_0 等)</param>
	/// <returns>指定キーがトリガー状態か</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号(DIK_0 等)</param>
	/// <returns>指定キーが押されているか</returns>
	bool PushKey(BYTE keyNumber);

private:	// シングルトンインスタンス
	static Input* instance;

	Input() = default;
	~Input() = default;
	Input(Input&) = default;
	Input& operator=(Input&) = delete;

private: // メンバ変数
	// キーボードのデバイス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	// DirectInputのインスタンス
	Microsoft::WRL::ComPtr<IDirectInput8> directInput;

	// WindowsAPI
	WindowsAPI* winApi_ = nullptr;

	// 前回の全キーの状態
	std::array<BYTE, 256> keyPre_;
	// 全キーの状態
	std::array<BYTE, 256> key_;
};

