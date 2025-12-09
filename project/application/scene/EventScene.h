#pragma once
#include "BaseScene.h"
#include <string>
#include "Sprite.h"
#include "MyBase.h"

/// <summary>
/// ゲーム中のイベント演出を管理するシーン。
/// </summary>
class EventScene : public BaseScene
{
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
	// テクスチャファイルパス


	// スプライト


	// モデルファイルパス
	

	// 3Dオブジェクト
	

	// パーティクル

	bool isParticleActive_;
	bool isAccelerationField_;
	MyBase::Vector3 acceleration_{};
	MyBase::AABB area_{};

	// デルタイム
	const float kDeltaTime = 1.0f / 60.0f;
};