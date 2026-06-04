#pragma once

/// <summary>
/// ポーズ画面専用の入力管理クラス
/// </summary>
class PauseInput
{
public:	// メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ESCが押されたか
	/// </summary>
	/// <returns></returns>
	bool IsPauseTriggered() const { return pauseTriggered_; };

	/// <summary>
	/// 上選択
	/// </summary>
	/// <returns></returns>
	bool IsSelectUp() const { return selectUp_; };

	/// <summary>
	/// 下選択
	/// </summary>
	/// <returns></returns>
	bool IsSelectDown() const { return selectDown_; };

	/// <summary>
	/// 決定
	/// </summary>
	/// <returns></returns>
	bool IsDecide() const { return decide_; };

private:	// メンバ変数
	bool pauseTriggered_ = false;
	bool selectUp_ = false;
	bool selectDown_ = false;
	bool decide_ = false;
};