#pragma once
#include <vector>
#include "MyBase.h"

/// <summary>
/// レールカメラクラス
/// あらかじめ設定されたレール(軌道)に沿って自動で移動・回転するカメラを制御するクラス。
/// </summary>
class RailCamera
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="points">レールの制御点リスト</param>
	void Initialize(const std::vector<MyBase::Vector3>& points);

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
#endif // _DEBUG

private:	// メンバ関数
	/// <summary>
	/// レールに沿って移動
	/// </summary>
	/// <param name="deltaTime">デルタタイム</param>
	void MoveAlongRail(float deltaTime);

public:	// getter
	/// <summary>
	/// レール上の現在位置を取得
	/// </summary>
	/// <returns>レール上の現在位置</returns>
	MyBase::Vector3 GetRailPosition() const { return railPosition_; }

	/// <summary>
	/// レールの向きを取得
	/// </summary>
	/// <returns>レールの向き</returns>
	MyBase::Vector3 GetRailDirection() const { return railDirection_; }

public:	// setter


private:	// メンバ変数
	std::vector<MyBase::Vector3> controlPoints_;	// レールの制御点リスト

	uint32_t currentPointIndex_ = 0;	// 現在の制御点インデックス
	uint32_t nextPointIndex_ = 1;		// 次の制御点インデックス

	float t_ = 0.0f;					// 区間内の補間値 0～1
	float speed_ = 0.05f;				// レールの進行速度

	MyBase::Vector3 railPosition_;		// レール上の現在位置
	MyBase::Vector3 railDirection_;		// レールの向き

	// 定数
	const size_t kNumRailPoints_ = 5; // レールの制御点の最大数

#ifdef _DEBUG
	bool isDebugMode_ = false;	// デバッグモードフラグ
#endif // _DEBUG

};

