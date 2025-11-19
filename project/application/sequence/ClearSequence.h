#pragma once

/// <summary>
/// ゲームクリア後に再生されるゲームクリア演出を管理するクラスです。
/// 一連の演出処理(エンジン起動 → 発進 → 演出 → 終了)をステートマシンとして制御し、
/// 時間経過に応じてテキストの表示や自機の移動などを管理します。
/// </summary>
class ClearSequence
{
private:	// enum
	// 演出ステート
	enum class State {
		EngineStart,
		Launch,
		Effect,
		Finished
	};

public:	// メンバ関数
	ClearSequence();
	~ClearSequence() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaTime">前の更新からの経過時間(秒)</param>
	void Update(float deltaTime);

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
	/// 自機の移動状態を経過時間に応じて更新
	/// </summary>
	/// <param name="deltaTime">前フレームからの経過時間(秒)。移動の更新量を計算するために使用</param>
	void UpdateShipMovement(float deltaTime);

	/// <summary>
	/// テキストを非表示
	/// </summary>
	void HideText();

private:	// メンバ変数
	State currentState_ = State::EngineStart;	// 現在のステート
	float stateTimer_ = 0.0f;					// ステート内の経過時間
	bool isFinished_ = false;					// 演出終了判定
};

