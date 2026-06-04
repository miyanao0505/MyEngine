#pragma once
#include <memory>

// 前方宣言
class PauseUI;
class PauseInput;

/// <summary>
/// ポーズ状態を管理するコントローラクラス。
/// ポーズOn/Off、UIと入力の統括、GameSceneとの接点
/// </summary>
class PauseController
{
public:	// メンバ関数
	PauseController();
	~PauseController();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画(ポーズ中のみ)
	/// </summary>
	void Draw();

	/// <summary>
	/// ポーズ中か
	/// </summary>
	/// <returns></returns>
	bool IsPaused() const { return isPaused_; }

private:	// メンバ関数
	/// <summary>
	/// ポーズ開始
	/// </summary>
	void EnterPause();

	/// <summary>
	/// ポーズ終了
	/// </summary>
	void ExitPause();

private:	// メンバ変数
	bool isPaused_ = false;

	std::unique_ptr<PauseUI> pauseUI_;
	std::unique_ptr<PauseInput> pauseInput_;
};