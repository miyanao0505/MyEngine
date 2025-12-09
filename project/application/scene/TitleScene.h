#pragma once
#include "BaseScene.h"
#include <string>
#include "Sprite.h"
#include "Skydome.h"
#include "TitleLogo.h"
#include "ParticleEmitter.h"

/// <summary>
/// ゲーム開始時に表示される**タイトルシーン**を管理するクラスです。
/// BaseSceneを継承し、シーンの初期化・更新・描画・終了処理を統括します。
/// </summary>
class TitleScene : public BaseScene
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

	///	<summary>
	/// 描画
	///	</summary>
	void Draw() override;

#ifdef _DEBUG
	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw() override;
#endif // _DEBUG

	/// <summary>
	// jsonファイルの読み込み
	/// </summary>
	///	<param name="filePath">ファイルパス</param>
	void LoadJsonFile([[maybe_unused]] const std::string& filePath) override;

private:	// メンバ変数
	// フェード用スプライト
	std::unique_ptr<Sprite> fadeSprite_ = nullptr;

	// スプライト

	// Skydome
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// 3Dオブジェクト
	std::unique_ptr<TitleLogo> titleLogo_ = nullptr;

	// jsonローダー
	std::unique_ptr<JsonLoader> jsonLoader_ = nullptr;

	// パーティクル
	
	
};