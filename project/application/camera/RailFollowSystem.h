#pragma once
#include "RailCamera.h"
#include "FollowCamera.h"
#include "Camera.h"

/// <summary>
/// レール追従システムクラス。
/// レール動作以外の追従位置を合成するシステムクラス。
/// </summary>
class RailFollowSystem
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="rail">レールカメラ</param>
	/// <param name="follow">フォローカメラ</param>
	void Initialize(RailCamera* rail, FollowCamera* follow);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:	// getter
	/// <summary>
	/// プレイヤーの位置の取得
	/// </summary>
	/// <returns>プレイヤーの位置</returns>
	MyBase::Vector3 GetPlayerPosition() const { return playerPos_; }

	/// <summary>
	/// 終了判定の取得
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const { return isFinished_; }

public:	// setter
	/// <summary>
	/// プレイヤーの位置の設定
	/// </summary>
	/// <param name="playerPos">プレイヤーの位置</param>
	void SetPlayerPosition(const MyBase::Vector3& playerPos) { playerPos_ = playerPos; }

	/// <summary>
	/// プレイヤーの速度の設定
	/// </summary>
	/// <param name="speed">プレイヤーの速度</param>
	void SetPlayerSpeed(float speed) { playerSpeed_ = speed; }

	/// <summary>
	/// 入力の設定
	/// </summary>
	/// <param name="input">入力</param>
	void SetInput(const MyBase::Vector2& input) { input_ = input; }

	/// <summary>
	/// X方向の最大オフセットの設定
	/// </summary>
	/// <param name="maxOffsetX">X方向の最大オフセット</param>
	void SetMaxOffsetX(float maxOffsetX) { maxOffsetX_ = maxOffsetX; }

	/// <summary>
	/// Y方向の最大オフセットの設定
	/// </summary>
	/// <param name="maxOffsetY">Y方向の最大オフセット</param>
	void SetMaxOffsetY(float maxOffsetY) { maxOffsetY_ = maxOffsetY; }

private:	// メンバ変数
	RailCamera* railCamera_ = nullptr;		// レールカメラ
	FollowCamera* followCamera_ = nullptr;	// 追従カメラ

	// 入力合成用
	MyBase::Vector2 input_;			// -1.0f ～ 1.0f の範囲で入力される想定
	float maxOffsetX_ = 15.0f;		// X方向の最大オフセット
	float maxOffsetY_ = 10.0f;		// Y方向の最大オフセット
	MyBase::Vector3 offsetX_;		// X方向のオフセット
	MyBase::Vector3 offsetY_;		// Y方向のオフセット
	MyBase::Vector3 offset_;		// 現在のオフセット

	MyBase::Vector3 playerPos_;	// プレイヤーの位置
	float playerSpeed_;			// プレイヤーの速度

	bool isFinished_ = false;	// レール追従終了フラグ
};