#pragma once
#include "BaseScene.h"
#include <string>
#include <vector>
#include "Sprite.h"
#include "Player.h"
#include "FollowCamera.h"
#include "RailCamera.h"
#include "RailFollowSystem.h"
#include "Enemy.h"
#include "BulletManager.h"
#include "Skydome.h"
#include "StartSequence.h"
#include "PauseController.h"
#include "ParticleEmitter.h"
#include "MyBase.h"
#ifdef _DEBUG
#include <imgui.h>
#include "DebugLineManager.h"
#endif // _DEBUG

/// <summary>
/// プレイヤーが実際に操作し、敵との戦闘や進行を行うゲーム本編のメインシーンを管理するクラス。
/// BaseSceneを継承し、ゲームプレイ中のキャラクター制御、カメラ挙動、敵出現、演出などを統括します。
/// </summary>
class GameScene : public BaseScene
{
private:
#pragma region 定数
	// プレイヤー
	static const MyBase::Vector3 kPlayerInitialTranslate;

	// 敵
	static const std::vector<MyBase::Vector3> kEnemyInitialTranslates;

	// カメラ初期設定
	static constexpr MyBase::Vector3 kCameraTranslate{ 0.0f, 5.0f,-40.0f };
	static constexpr MyBase::Vector3 kCameraRotate{ 0.0f, 0.0f, 0.0f };

	// ライト初期設定
	static constexpr MyBase::Vector4 kClearLightColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	static constexpr MyBase::Vector3 kClearLightPos{ 0.0f, 0.0f, -40.0f };
	static constexpr float kClearLightIntensity = 1.0f;
	static constexpr float kClearLightRadius = 500.0f;
	static constexpr float kClearLightDecay = 2.0f;

	// Skydome初期設定
	static constexpr MyBase::Vector3 kSkydomeTranslate{ 0.0f, 0.0f, 0.0f };
	static constexpr MyBase::Vector3 kSkydomeScale{ 800.0f, 800.0f, 800.0f };

	// ゲームクリア演出時間
	static constexpr float kGameClearDuration = 1.0f;

	// ゲームオーバー演出時間
	static constexpr float kGameOverDuration = 1.0f;

	// パーティクル加速フィールド初期設定
	static constexpr MyBase::Vector3 kAcceleration{ 15.0f, 0.0f, 0.0f };
	static constexpr MyBase::AABB kAccelArea{ .min{ -1.0f, -1.0f, -1.0f }, .max{1.0f, 1.0f, 1.0f} };

	// レール通過点
	static const std::vector<MyBase::Vector3> kRailPoints;
#ifdef _DEBUG
	// ImGui
	static constexpr ImVec2 kDebugWindowPosScene{ 20.0f, 350.0f };
	static constexpr ImVec2 kDebugWindowSizeScene{ 350.0f, 150.0f };
	static constexpr ImVec2 kDebugWindowPosSettings{ 900.0f, 20.0f };
	static constexpr ImVec2 kDebugWindowSizeSettings{ 350.0f, 150.0f };
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
	///	</summary>
	void DebugUpdate();

	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw() override;

	/// <summary>
	/// デバッグライン追加
	/// </summary>
	void AddDebugLines();
#endif // _DEBUG

	/// <summary>
	/// jsonファイルの読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadJsonFile([[maybe_unused]] const std::string& filePath) override;

private:	// メンバ変数
#pragma region スプライト
	// UI
	std::unique_ptr<Reticle> reticle_ = nullptr;
	std::unique_ptr<Sprite> escapeUI_ = nullptr;
#pragma endregion

#pragma region 3Dオブジェクト
	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	// 敵
	std::list<std::unique_ptr<Enemy>> enemies_;
	// 弾管理
	BulletManager* bulletManager_ = nullptr;
	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	// 3DReticle
	std::unique_ptr<BaseObject> reticle3D_ = nullptr;
#pragma endregion

	// カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
	std::unique_ptr<RailCamera> railCamera_ = nullptr;
	std::unique_ptr<RailFollowSystem> railFollowSystem_ = nullptr;
	
	// スポーンポイント
	std::vector<std::unique_ptr<MyBase::PlayerSpawnData>> spawnPoints_;

	// シーケンス
	std::unique_ptr<StartSequence> startSequence_ = nullptr;

	// ポーズ管理
	std::unique_ptr<PauseController> pauseController_ = nullptr;

	// パーティクル

	// jsonローダー
	std::unique_ptr<JsonLoader> jsonLoader_ = nullptr;
	
	bool isParticleActive_;
	bool isAccelerationField_;
	MyBase::Vector3 acceleration_{};
	MyBase::AABB area_{};

	bool isGameClear_ = false;
	float gameClearTimer_ = 0.0f;

	bool isGameOver_ = false;
	float gameOverTimer_ = 0.0f;
};