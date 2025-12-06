#pragma once
#include <memory>
#include <string>
#include "BaseObject.h"

/// <summary>
/// プレイヤーの敗北後に再生されるゲームオーバー演出を管理するクラスです。
/// 一連の演出処理(エンジン起動 → 爆発 → 落下 → 終了)をステートマシンとして制御し、
/// 時間経過に応じてテキストの表示や回転アニメーションなどを管理します。
/// </summary>
class GameOverSequence
{
private:	// enum
	// 演出ステート
	enum class State {
		EngineStart,
		Explosion,
		Fall,
		Finished
	};

public:	// メンバ関数
	GameOverSequence();
	~GameOverSequence() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaTime">前の更新からの経過時間(秒)</param>
	void Update([[maybe_unused]] float deltaTime);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:	// メンバ関数
	/// <summary>
	/// エンジン起動時のエフェクトを再生
	/// </summary>
	void EngineStartEffect();
	/// <summary>
	/// テキストの設定
	/// </summary>
	void TextSet();
	/// <summary>
	/// テキストの回転状態を経過時間に応じて更新
	/// </summary>
	/// <param name="deltaTime">前フレームからの経過時間(秒)。回転の更新量を計算するために使用</param>
	void TextRotate(float deltaTime);
	/// <summary>
	/// テキストを非表示
	/// </summary>
	void HideText();

public:	// getter
	/// <summary>
	/// 演出終了判定
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const { return isFinished_; }

private:	// メンバ変数
	State state_ = State::EngineStart;	// 現在のステート
	float timer_ = 0.0f;				// タイマー
	bool isFinished_ = false;			// 演出終了判定

	bool engineStarted_ = false;		// エンジン始動済み判定
	bool textSwitched_ = false;			// テキスト切り替え済み判定
	float rotationY_ = 0.0f;			// ミッション回転用Y軸角度

	std::unique_ptr<BaseObject> missionText_ = nullptr; // ミッションテキスト
	std::unique_ptr<BaseObject> startText_ = nullptr;   // スタートテキスト
};

