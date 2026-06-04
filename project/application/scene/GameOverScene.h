#pragma once
#include "BaseScene.h"
#include <string>
#include "Skydome.h"
#include "GameOverLogo.h"
#include "MyBase.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

/// <summary>
/// プレイヤーが敗北した際に表示されるゲームオーバー演出シーンを管理するクラス。
/// BaseSceneを継承し、ゲームオーバー時のビジュアル演出やシーン遷移制御を担当します。
/// </summary>
class GameOverScene : public BaseScene
{
private:
#pragma region 定数
	// カメラ初期設定
	static constexpr MyBase::Vector3 kCameraTranslate{ 0.0f, 0.0f,-40.0f };
	static constexpr MyBase::Vector3 kCameraRotate{ 0.0f, 0.0f, 0.0f };

	// ライト初期設定
	static constexpr MyBase::Vector4 kClearLightColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	static constexpr MyBase::Vector3 kClearLightPos{ 0.0f, 0.0f, -40.0f };
	static constexpr float kClearLightIntensity = 1.0f;
	static constexpr float kClearLightRadius = 500.0f;
	static constexpr float kClearLightDecay = 2.0f;

	// Skydome初期設定
	static constexpr MyBase::Vector3 kSkydomeTranslate{ 0.0f, 0.0f, 0.0f };
	static constexpr MyBase::Vector3 kSkydomeScale{ 100.0f, 100.0f, 100.0f };

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
	// スプライト

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// 3Dオブジェクト
	// ロゴ
	std::unique_ptr<GameOverLogo> gameOverLogo_ = nullptr;

	// パーティクル

	bool isParticleActive_;
	bool isAccelerationField_;
	MyBase::Vector3 acceleration_{};
	MyBase::AABB area_{};
};