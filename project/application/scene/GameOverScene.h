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
	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;

#ifdef _DEBUG
	// デバッグ更新
	void DebugUpdate();

	// デバッグ描画
	void DebugDraw() override;
#endif // _DEBUG

	// jsonファイルの読み込み
	void LoadJsonFile(const std::string& filePath) override;

private:	// メンバ変数
#pragma region シーン初期化
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
#pragma endregion シーン初期化

	bool isParticleActive_;
	bool isAccelerationField_;
	MyBase::Vector3 acceleration_{};
	MyBase::AABB area_{};

	// デルタイム
	const float kDeltaTime_ = 1.0f / 60.0f;
};

