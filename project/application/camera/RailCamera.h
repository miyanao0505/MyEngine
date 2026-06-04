#pragma once
#include <vector>
#include "MyBase.h"

/// <summary>
/// レールカメラクラス
/// あらかじめ設定されたレール(軌道)に沿って自動で移動・回転するカメラを制御するクラス。
/// </summary>
class RailCamera
{
private:	// 構造体
	struct ArcLengthSample {
		float t;
		float length;
	};

private:
#pragma region 定数
	static constexpr float kDefaultRailSpeed = 20.0f;	// レールの進行速度
	static constexpr size_t kMinCatmullRomPoints = 4;	// Catmull-Rom点の最小数

	static constexpr float kLerpEnd = 1.0f;
	static constexpr float kLerpStart = 0.0f;

	static constexpr float kForwardSampleOffset = 0.001f;	// 接線方向計算用のサンプリングオフセット値

	static constexpr int kDiv = 300;	// アーク長テーブルの分割数
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

	/// <summary>
	/// アーク長テーブルの構築
	/// </summary>
	void BuildArcTable();

	/// <summary>
	/// 距離から区間内の補間値を求める
	/// </summary>
	/// <param name="distance">距離</param>
	/// <returns>区間内の補間値</returns>
	float DistanceToT(float distance);

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

	/// <summary>
	/// レール上の現在距離を取得
	/// </summary>
	/// <returns>レール上の現在距離</returns>
	float GetCurrentDistance() const { return currentDistance_; }

	/// <summary>
	/// レールの全長を取得
	/// </summary>
	/// <returns>レールの全長</returns>
	float GetTotalLength() const { return totalLength_; }

	/// <summary>
	/// レール終了判定
	/// </summary>
	/// <returns>レール終了判定</returns>
	bool IsFinished() const { return currentDistance_ >= totalLength_; }

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
	void SetRailPoints(const std::vector<MyBase::Vector3>& points);

private:	// メンバ変数
	std::vector<MyBase::Vector3> controlPoints_;	// レールの制御点リスト

	float lerpT_;			// 区間内の補間値 0～1
	float speed_;			// レールの進行速度

	float currentDistance_ = 0.0f;	// レール上の現在距離
	float totalLength_ = 0.0f;		// レールの全長

	MyBase::Vector3 position_;	// レール上の現在位置
	MyBase::Vector3 forward_;	// レール上の現在向き

	std::vector<ArcLengthSample> arcTable_;	// アーク長テーブル

#ifdef _DEBUG
	bool isDebugMode_ = false;	// デバッグモードフラグ
#endif // _DEBUG
};