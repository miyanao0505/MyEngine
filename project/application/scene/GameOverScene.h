#pragma once
#include "BaseScene.h"
#include <string>
#include <vector>
#include "Sprite.h"
#include "Object3d.h"
#include "Skydome.h"
#include "GameOverLogo.h"
#include "ParticleEmitter.h"
#include "MyBase.h"

/// <summary>
/// プレイヤーが敗北した際に表示されるゲームオーバー演出シーンを管理するクラス。
/// BaseSceneを継承し、ゲームオーバー時のビジュアル演出やシーン遷移制御を担当します。
/// </summary>
class GameOverScene : public BaseScene
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
	void LoadJsonFile(const std::string& filePath) override;

private:	// メンバ変数
	// テクスチャファイルパス
	std::string skydomeFilePath_ = "resources/texture/skyback.png";

	// スプライト

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// 3Dオブジェクト
	// ロゴ
	std::unique_ptr<GameOverLogo> gameOverLogo_ = nullptr;

	// パーティクル
	//std::unique_ptr<ParticleEmitter> particleEmitter_ = nullptr;

	bool isParticleActive_;
	bool isAccelerationField_;
	MyBase::Vector3 acceleration_{};
	MyBase::AABB area_{};

	// デルタイム
	const float kDeltaTime_ = 1.0f / 60.0f;
};

