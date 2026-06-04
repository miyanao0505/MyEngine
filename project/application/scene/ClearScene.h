#pragma once
#include "BaseScene.h"
#include <string>
#include <vector>
#include "Sprite.h"
#include "Object3d.h"
#include "Player.h"
#include "Skydome.h"
#include "ClearLogo.h"
#include "ParticleEmitter.h"
#include "MyBase.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

/// <summary>
/// ゲームクリア時に表示されるシーンを管理するクラス。
/// BaseSceneを継承し、クリア演出の表示・更新・描画処理を担当します。
/// </summary>
class ClearScene : public BaseScene
{
private:
#pragma region 定数
	// プレイヤー初期設定
	static constexpr MyBase::Vector3 kPlayerTranslate{ 0.0f, 0.0f, 0.0f };
	static constexpr MyBase::Vector3 kPlayerScale{ 0.75f, 0.75f, 0.75f };

	// カメラ初期設定
	static constexpr MyBase::Vector3 kCameraTranslate{ 0.0f, 5.0f,-30.0f };
	static constexpr MyBase::Vector3 kCameraRotate{ 0.150f, 0.0f, 0.0f };

	// ライト初期設定
	static constexpr MyBase::Vector4 kClearLightColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	static constexpr MyBase::Vector3 kClearLightPos{ 0.0f, -5.0f, -40.0f };
	static constexpr float kClearLightIntensity = 1.0f;
	static constexpr float kClearLightRadius = 500.0f;
	static constexpr float kClearLightDecay = 0.50f;

	// Skydome初期設定
	static constexpr MyBase::Vector3 kSkydomeTranslate{ 0.0f, 0.0f, 0.0f };
	static constexpr MyBase::Vector3 kSkydomeScale{ 100.0f, 100.0f, 100.0f };

	// パーティクル加速フィールド初期設定
	static constexpr MyBase::Vector3 kAcceleration{ 15.0f, 0.0f, 0.0f };
	static constexpr MyBase::AABB kAccelArea{ .min{ -1.0f, -1.0f, -1.0f }, .max{1.0f, 1.0f, 1.0f} };
#ifdef _DEBUG
	// ImGui
	static constexpr ImVec2 kDebugWindowPos{ 20.0f, 350.0f };
	static constexpr ImVec2 kDebugWindowSize{ 350.0f, 150.0f };
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
	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	
	// ロゴ
	std::unique_ptr<ClearLogo> clearLogo_ = nullptr;

	// パーティクル

	bool isParticleActive_;
	bool isAccelerationField_;
	MyBase::Vector3 acceleration_{};
	MyBase::AABB area_{};
};