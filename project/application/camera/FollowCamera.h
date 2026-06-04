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
	static constexpr float kDefaultFollowSmooth = 5.0f;

#ifdef _DEBUG
	static constexpr const char* kOffsetLabel = "Offset";
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

	/// <summary>
	/// オフセットの方向の設定
	/// </summary>
	/// <param name="right">左右</param>
	/// <param name="up">上下</param>
	/// <param name="forward">前後</param>
	void SetBasis(MyBase::Vector3 right, MyBase::Vector3 up, MyBase::Vector3 forward) { basisRight_ = right; basisUp_ = up; basisForward_ = forward; }

private:	// メンバ変数
	Camera* camera_ = nullptr;

	MyBase::Vector3 target_;	// 追従対象(外部からセット)
	MyBase::Vector3 offset_;	// ターゲットからの相対位置
	float followSmooth_;		// 補間係数
	
	MyBase::Vector3 basisRight_;		// オフセットの基準座標系の右方向
	MyBase::Vector3 basisUp_;			// オフセットの基準座標系の上方向
	MyBase::Vector3 basisForward_;		// オフセットの基準座標系の前方向

	bool isTargetSet_ = false;	// 追従対象がセットされたかどうか

	// カメラの方向
	MyBase::Vector3 upDirection_ = kWorldUp;

	// プレイヤーを追っただけの理想位置
	MyBase::Vector3 currentFollowPosition_;
};