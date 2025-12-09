#pragma once
#include <functional>
#include <string>
#include <memory>
#include "Sprite.h"

/// <summary>
/// シーン切替フェード演出クラス
/// ゲームシーンの切り替え時に、フェードアウト → シーン変更 → フェードイン の演出を行うクラス。
/// </summary>
class SceneTransition
{
public:
	using FinishCallback = std::function<void()>; // フェード完了コールバック関数の型定義

public:	// メンバ関数
	/// <summary>
	/// フェード演出の開始処理
	/// </summary>
	/// <param name="onSwitch">シーン切替コールバック関数</param>
	void Start(FinishCallback onSwitch);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();    // αを使って黒フェードを描画
	
	/// <summary>
	/// 演出中かどうかを取得
	/// </summary>
	/// <returns>演出中なら true、停止中なら false</returns>
	bool IsActive() const { return phase_ != Phase::kNone; }

private:
	enum class Phase {
		kNone,
		kFadeIn,		// 暗転
		kFadeOut,	// 明転
	};

private:	// メンバ変数
	Phase phase_ = Phase::kNone;
	float timer_ = 0.0f;        // タイマー

	static constexpr float kFadeTime = 1.0f;

	FinishCallback onSwitch_ = nullptr; // シーン切替タイミングのコールバック

	// フェード用スプライト
	std::unique_ptr<Sprite> fadeSprite_ = nullptr;
};