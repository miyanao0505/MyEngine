#pragma once
#include <vector>
#include "MyBase.h"

/// <summary>
/// レールカメラクラス
/// あらかじめ設定されたレール(軌道)に沿って自動で移動・回転するカメラを制御するクラス。
/// </summary>
class RailCamera
{
private:
#pragma region 定数
	static constexpr float kDefaultRailSpeed = 0.075f;	// レールの進行速度
	static constexpr size_t kMinCatmullRomPoints = 4;	// Catmull-Rom点の最小数

	static constexpr float kLerpEnd = 1.0f;
	static constexpr float kLerpStart = 0.0f;

	static constexpr float kForwardSampleOffset = 0.001f;	// 接線方向計算用のサンプリングオフセット値
#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaTime">デルタタイム</param>
	void Update(float deltaTime);

#ifdef _DEBUG
	/// <summary>
	/// デバッグ更新
	/// </summary>
	void DebugUpdate();
	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw();
#endif // _DEBUG

private:	// メンバ関数
	/// <summary>
	/// レールに沿って移動
	/// </summary>
	/// <param name="deltaTime">デルタタイム</param>
	void MoveAlongRail(float deltaTime);

public:	// getter
	/// <summary>
	/// レールデータの取得
	/// </summary>
	/// <returns>レールデータの取得</returns>
	MyBase::RailData GetRailData() const;

	/// <summary>
	/// レール上の現在位置を取得
	/// </summary>
	/// <returns>レール上の現在位置</returns>
	MyBase::Vector3 GetRailPosition() const { return position_; }

	/// <summary>
	/// レールの向きを取得
	/// </summary>
	/// <returns>レールの向き</returns>
	MyBase::Vector3 GetRailDirection() const { return forward_; }

	/// <summary>
	/// レールの補間値を取得
	/// </summary>
	/// <returns></returns>
	float GetRailLerpT() const { return lerpT_; }

#ifdef _DEBUG
	/// <summary>
	/// デバッグモードフラグ
	/// </summary>
	/// <returns>デバッグモードON:ture / OFF:false</returns>
	bool IsDebugMode() const { return isDebugMode_; }
#endif // _DEBUG

public:	// setter
	/// <summary>
	/// レールの制御点リストを設定
	/// </summary>
	/// <param name="points">レールの制御点</param>
	void SetRailPoints(const std::vector<MyBase::Vector3>& points) { controlPoints_ = points; }

private:	// メンバ変数
	std::vector<MyBase::Vector3> controlPoints_;	// レールの制御点リスト

	float lerpT_;			// 区間内の補間値 0～1
	float speed_;			// レールの進行速度

	MyBase::Vector3 position_;	// レール上の現在位置
	MyBase::Vector3 forward_;	// レール上の現在向き

#ifdef _DEBUG
	bool isDebugMode_ = false;	// デバッグモードフラグ
#endif // _DEBUG
};