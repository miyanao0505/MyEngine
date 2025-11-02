#pragma once
#include "Camera.h"
#include "Player.h"
#include "MyBase.h"

/// <summary>
/// プレイヤー追従カメラクラス
/// プレイヤーの動きを追従するタイプのカメラを制御するクラス。
/// </summary>
class FollowCamera {
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	///  描画
	/// </summary>
	void Draw();

#ifdef _DEBUG
	// デバック用の描画
	void DebugDraw();
#endif // _DEBUG

public:	// getter
	/// <summary>
	/// カメラの設定
	/// </summary>
	/// <param name="camera">担当のカメラ</param>
	void SetCamera(Camera* camera) { camera_ = camera; }
	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="player">担当のプレイヤー</param>
	void SetPlayer(Player* player) { player_ = player; }

public:	// setter

private:	// メンバ変数
	Camera* camera_ = nullptr;

	Player* player_ = nullptr;
	
	// 注視点からのオフセット
	MyBase::Vector3 offset_;
	// カメラの方向
	MyBase::Vector3 directional_;
};

