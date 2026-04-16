#pragma once
#include "DebugLineCategory.h"
#include "DebugLineLayer.h"
#include "DebugLineDrawer.h"
#include "MyBase.h"
#include <memory>

/// <summary>
/// デバッグライン管理クラス
/// デバッグラインの管理を行うクラス。
/// </summary>
class DebugLineManager {
public:	// メンバ関数
	/// <summary>
	/// Singleton Instance を取得
	/// </summary>
	/// <returns>DebugLineManager</returns>
	static DebugLineManager* GetInstance();

	/// ------ Passkey Idion ------
	/// コントラクタを渡すための鍵
	class ConstructorKey {
	private:
		ConstructorKey() = default;
		friend class DebugLineManager;
	};

	/// PassKeyを受け取るコンストラクタ
	explicit DebugLineManager(ConstructorKey) {}

	/// コピー禁止
	DebugLineManager(const DebugLineManager&) = delete;
	DebugLineManager& operator=(const DebugLineManager&) = delete;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// フレーム開始時に呼ぶ
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// ラインの追加
	/// </summary>
	/// <param name="category">デバッグラインカテゴリ</param>
	/// <param name="line">追加するデバッグライン</param>
	void AddLine(DebugLineCategory category, const MyBase::DebugLine& line);

	/// <summary>
	/// ラインの描画
	/// </summary>
	/// <param name="category">描画するデバッグラインのカテゴリ</param>
	void Draw(DebugLineCategory category);

	/// <summary>
	/// 直線の描画
	/// </summary>
	/// <param name="category">描画するデバッグラインのカテゴリ</param>
	/// <param name="line">描画するデバッグライン</param>
	void DrawLine(DebugLineCategory category, const MyBase::DebugLine& line);

	/// <summary>
	/// 直線の描画
	/// </summary>
	/// <param name="category">描画するデバッグラインのカテゴリ</param>
	/// <param name="start">直線の開始点</param>
	/// <param name="end">直線の終了点</param>
	/// <param name="color">直線の色</param>
	void DrawLine(DebugLineCategory category, const MyBase::Vector3& start, const MyBase::Vector3& end, const MyBase::Vector4& color);

	/// <summary>
	/// スフィアの描画
	/// </summary>
	/// <param name="category">描画するデバッグラインのカテゴリ</param>
	/// <param name="sphere">描画するスフィア</param>
	/// <param name="color">スフィアの色</param>
	/// <param name="segment">スフィアの分割数</param>
	/// <param name="isHit">スフィアがヒットしているかどうか</param>
	void DrawSphere(DebugLineCategory category, const MyBase::Sphere& sphere, const MyBase::Vector4& color, uint32_t segment = 16, bool isHit = false);

	/// <summary>
	/// スフィアの描画
	/// </summary>
	/// <param name="category">描画するデバッグラインのカテゴリ</param>
	/// <param name="center">スフィアの中心位置</param>
	/// <param name="radius">スフィアの半径</param>
	/// <param name="color">スフィアの色</param>
	/// <param name="segment">スフィアの分割数</param>
	/// <param name="isHit">スフィアがヒットしているかどうか</param>
	void DrawSphere(DebugLineCategory category, const MyBase::Vector3& center, float radius, const MyBase::Vector4& color, uint32_t segment = 16, bool isHit = false);

	/// <summary>
	/// ボックスの描画
	/// </summary>
	/// <param name="category">描画するデバッグラインのカテゴリ</param>
	/// <param name="center">ボックスの中心位置</param>
	/// <param name="size">ボックスのサイズ</param>
	/// <param name="worldMatrix">ボックスのワールド行列</param>
	/// <param name="color">ボックスの色</param>
	/// <param name="isHit">ボックスがヒットしているかどうか</param>
	void DrawBox(DebugLineCategory category, const MyBase::Vector3& center, const MyBase::Vector3& size, const MyBase::Matrix4x4& worldMatrix, const MyBase::Vector4& color, bool isHit = false);

	/// <summary>
	/// OBBの描画
	/// </summary>
	/// <param name="category">描画するデバッグラインのカテゴリ</param>
	/// <param name="obb">描画するOBB</param>
	/// <param name="color">OBBの色</param>
	/// <param name="isHit">OBBがヒットしているかどうか</param>
	void DrawOBB(DebugLineCategory category, const MyBase::OBB& obb, const MyBase::Vector4& color, bool isHit = false);

	/// <summary>
	/// スプラインの描画
	/// </summary>
	/// <param name="category">描画するデバッグラインのカテゴリ</param>
	/// <param name="points">スプラインを構成するポイントの配列</param>
	/// <param name="division">スプラインの分割数</param>
	/// <param name="color">スプラインの色</param>
	/// <param name="isHit">スプラインがヒットしているかどうか</param>
	void DrawSpline(DebugLineCategory category, const std::vector<MyBase::Vector3>& points, uint32_t division, const MyBase::Vector4& color, bool isHit = false);

	/// <summary>
	/// 全てのラインの描画
	/// </summary>
	void DrawAll();

public:	// getter

public:	// setter
	/// <summary>
	/// デバッグラインの有効/無効設定
	/// </summary>
	/// <param name="cat">デバッグラインのカテゴリ</param>
	/// <param name="enable">有効/無効フラグ</param>
	void SetEnable(DebugLineCategory cat, bool enable) { enableFlags_[size_t(cat)] = enable; }

public:	// Singleton Instance
	static std::unique_ptr<DebugLineManager> sInstance_;

private:	// メンバ変数
	std::array<DebugLineLayer, size_t(DebugLineCategory::kCount)> layers_;
	std::array<bool, size_t(DebugLineCategory::kCount)> enableFlags_;
	std::vector<MyBase::DebugLine> workLines_;	// ライン生成用のワークスペース

	std::unique_ptr<DebugLineDrawer> drawer_;
};