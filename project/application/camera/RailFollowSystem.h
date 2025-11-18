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
	/// <param name="outCamera">カメラ</param>
	void Initialize(RailCamera* rail, FollowCamera* follow, Camera* outCamera);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:	// getter


public:	// setter
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
	Camera* camera_ = nullptr;				// 出力カメラ

	float maxOffsetX_ = 5.0f;			// X方向の最大オフセット
	float maxOffsetY_ = 3.0f;			// Y方向の最大オフセット
};

