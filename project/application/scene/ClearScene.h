#pragma once
#include "BaseScene.h"
#include <string>
#include <vector>
#include "Sprite.h"
#include "Object3d.h"
#include "Skydome.h"
#include "ClearLogo.h"
#include "ParticleEmitter.h"
#include "MyBase.h"

/// <summary>
/// ゲームクリア時に表示されるシーンを管理するクラス。
/// BaseSceneを継承し、クリア演出の表示・更新・描画処理を担当します。
/// </summary>
class ClearScene : public BaseScene
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
	// スプライト

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// 3Dオブジェクト
	// ロゴ
	std::unique_ptr<ClearLogo> clearLogo_ = nullptr;

	// パーティクル

	bool isParticleActive_;
	bool isAccelerationField_;
	MyBase::Vector3 acceleration_{};
	MyBase::AABB area_{};

	// デルタイム
	const float kDeltaTime = 1.0f / 60.0f;
};