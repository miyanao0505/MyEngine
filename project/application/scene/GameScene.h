#pragma once
#include "BaseScene.h"
#include <string>
#include <vector>
#include "Sprite.h"
#include "Player.h"
#include "FollowCamera.h"
#include "Enemy.h"
#include "Skydome.h"
#include "StartSequence.h"
#include "ParticleEmitter.h"
#include "MyBase.h"

/// <summary>
/// プレイヤーが実際に操作し、敵との戦闘や進行を行うゲーム本編のメインシーンを管理するクラス。
/// BaseSceneを継承し、ゲームプレイ中のキャラクター制御、カメラ挙動、敵出現、演出などを統括します。
/// </summary>
class GameScene : public BaseScene
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
	///	</summary>
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

	
	// 3Dオブジェクト
	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	// 敵
	std::unique_ptr<Enemy> enemy_ = nullptr;
	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
	
	// スポーンポイント
	std::vector<std::unique_ptr<MyBase::PlayerSpawnData>> spawnPoints_;

	// シーケンス
	std::unique_ptr<StartSequence> startSequence_ = nullptr;

	// 3Dオブジェクト

	// パーティクル

	// jsonローダー
	std::unique_ptr<JsonLoader> jsonLoader_ = nullptr;
	
	bool isParticleActive_;
	bool isAccelerationField_;
	MyBase::Vector3 acceleration_{};
	MyBase::AABB area_{};

	// デルタイム
	const float kDeltaTime_ = 1.0f / 60.0f;
};

