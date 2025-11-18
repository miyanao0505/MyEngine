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
	/// <param name="deltaTime">前フレームからの経過時間</param>
	void Update(float deltaTime);

	/// <summary>
	///  描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 追従処理を更新する
	/// </summary>
	/// <param name="deltaTime">前フレームからの経過時間</param>Vtuber
	void UpdateFollow(float deltaTime);

#ifdef _DEBUG
	// デバック用の描画
	void DebugDraw();
#endif // _DEBUG

public:	// getter
	/// <summary>
	/// プレイヤーとカメラのオフセットの取得
	/// </summary>
	/// <returns></returns>
	MyBase::Vector3 GetOffset() { return offset_; }
	/// <summary>
	/// プレイヤーを追っただけの理想位置の取得
	/// </summary>
	/// <returns></returns>
	MyBase::Vector3 GetRawFollowPosition() const { return rawFollowPosition_; }
	/// <summary>
	/// 注視点の位置の取得
	/// </summary>
	/// <returns></returns>
	MyBase::Vector3 GetRawLookAtPosition() const { return target_; }

public:	// setter
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
	/// <summary>
	/// プレイヤーとカメラのオフセットの設定
	/// </summary>
	/// <param name="offset"></param>
	void SetOffset(MyBase::Vector3 offset) { offset_ = offset; }

private:	// メンバ変数
	Camera* camera_ = nullptr;

	Player* player_ = nullptr;
	
	// 注視点からのオフセット
	MyBase::Vector3 offset_ = { 0.0f, 7.50f, -40.0f };
	// カメラの方向
	MyBase::Vector3 directional_ = { 0.0f, 1.0f, 0.0f };
	// カメラの追従のスムージング係数
	float followSmooth_ = 8.0f;

	// プレイヤーを追っただけの理想位置
	MyBase::Vector3 rawFollowPosition_;
	// 注視点の位置
	MyBase::Vector3 target_;
	// FollowCameraが動作するかしないかを切り替えるフラグ
	bool enable_ = true;
};

