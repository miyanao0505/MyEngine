#pragma once
#include "Camera.h"
#include "MyBase.h"

/// <summary>
/// プレイヤー追従カメラクラス
/// プレイヤーの動きを追従するタイプのカメラを制御するクラス。
/// </summary>
class FollowCamera {
private:
#pragma region 定数
	static constexpr MyBase::Vector3 kDefaultOffset{ 0.0f, 5.0f, -40.0f };
	static constexpr MyBase::Vector3 kWorldUp{ 0.0f, 1.0f, 0.0f };
	static constexpr float kDefaultFollowSmooth = 8.0f;

#ifdef _DEBUG
	static constexpr float kOffsetDragSpeed = 0.05f;
#endif // _DEBUG
#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">担当のカメラ</param>
	void Initialize(Camera* camera);
	
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaTime">前フレームからの経過時間</param>
	void Update(float deltaTime);

	/// <summary>
	/// 追従処理を更新する
	/// </summary>
	/// <param name="deltaTime">前フレームからの経過時間</param>
	void UpdateFollowTranslate( float deltaTime);

	/// <summary>
	/// 注視点の更新
	/// </summary>
	void UpdateLookAtTarget();

#ifdef _DEBUG
	/// <summary>
	/// デバック用の描画
	/// </summary>
	void DebugDraw();
#endif // _DEBUG

public:	// getter
	/// <summary>
	/// プレイヤーとカメラのオフセットの取得
	/// </summary>
	/// <returns>プレイヤーとカメラのオフセット</returns>
	MyBase::Vector3 GetOffset() const { return offset_; }

	/// <summary>
	/// プレイヤーを追っただけの理想位置の取得
	/// </summary>
	/// <returns>プレイヤーを追っただけの理想位置</returns>
	MyBase::Vector3 GetRawFollowPosition() const { return currentFollowPosition_; }
	
	/// <summary>
	/// 注視点の位置の取得
	/// </summary>
	/// <returns>注視点の位置</returns>
	MyBase::Vector3 GetRawLookAtPosition() const { return target_; }

public:	// setter
	/// <summary>
	/// カメラの設定
	/// </summary>
	/// <param name="camera">担当のカメラ</param>
	void SetCamera(Camera* camera) { camera_ = camera; }
	
	/// <summary>
	/// 追従対象の設定
	/// </summary>
	/// <param name="player">追従対象</param>
	void SetTargetPosition(const MyBase::Vector3& target) { target_ = target; isTargetSet_ = true; }
	
	/// <summary>
	/// プレイヤーとカメラのオフセットの設定
	/// </summary>
	/// <param name="offset"></param>
	void SetOffset(MyBase::Vector3 offset) { offset_ = offset; }

private:	// メンバ変数
	Camera* camera_ = nullptr;

	MyBase::Vector3 target_;	// 追従対象(外部からセット)
	MyBase::Vector3 offset_;	// ターゲットからの相対位置
	float followSmooth_;		// 補間係数
	
	bool isTargetSet_ = false;	// 追従対象がセットされたかどうか

	// カメラの方向
	MyBase::Vector3 upDirection_ = kWorldUp;

	// プレイヤーを追っただけの理想位置
	MyBase::Vector3 currentFollowPosition_;
};