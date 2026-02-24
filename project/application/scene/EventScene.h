#pragma once
#include "BaseScene.h"
#include <string>
#include "Sprite.h"
#include "MyBase.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

/// <summary>
/// ゲーム中のイベント演出を管理するシーン。
/// </summary>
class EventScene : public BaseScene
{
private:
#pragma region 定数
	// パーティクル加速フィールド初期設定
	static constexpr MyBase::Vector3 kAcceleration{ 15.0f, 0.0f, 0.0f };
	static constexpr MyBase::AABB kAccelArea{ .min{ -1.0f, -1.0f, -1.0f }, .max{1.0f, 1.0f, 1.0f} };
#ifdef _DEBUG
	// ImGuiウィンドウサイズ
	static constexpr ImVec2 kDebugWindowPos{ 20.0f, 350.0f };
	static constexpr ImVec2 kDebugWindowSize{ 350.0f, 150.0f };
#endif // _DEBUG
#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

#ifdef _DEBUG
	/// <summary>
	/// デバッグ更新
	/// </summary>
	void DebugUpdate();

	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw() override;
#endif // _DEBUG

	/// <summary>
	/// jsonファイルの読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadJsonFile([[maybe_unused]] const std::string& filePath) override;

private:	// メンバ変数
	// 今後イベント演出用に追加予定
	// (現在はベース構造の未実装)

	// テクスチャファイルパス


	// スプライト


	// モデルファイルパス
	

	// 3Dオブジェクト
	

	// パーティクル

};