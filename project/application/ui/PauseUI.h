#pragma once
#include <vector>
#include <memory>
#include "PauseButton.h"
#include "Sprite.h"
#include "PauseButtonDesc.h"

// 前方宣言
class PauseInput;

/// <summary>
/// ポーズ画面のUI全体を管理するクラス
/// </summary>
class PauseUI
{
private:
#pragma region 定数
	static constexpr MyBase::Vector2 kTranslate{ 0.0f, 0.0f };
	static constexpr MyBase::Vector2 kScale{ 1280.0f, 720.0f };
	static constexpr MyBase::Vector4 kColor{ 0.0f, 0.0f, 0.0f, 0.5f };

	static const PauseButtonDesc kResumeButton;
	static const PauseButtonDesc kGoToTitleButton;
#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力</param>
	void Update(const PauseInput& input);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ポーズ開始時
	/// </summary>
	void OnOpen();

	/// <summary>
	/// ポーズ終了時
	/// </summary>
	void OnClose();

	/// <summary>
	/// 決定か
	/// </summary>
	/// <returns>決定</returns>
	bool HasDecided() const { return decided_; }

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	PauseButtonType GetDecidedButton() const;

private:	// メンバ関数
	/// <summary>
	/// 選択移動
	/// </summary>
	/// <param name="dir">方向</param>
	void MoveSelection(int dir);

private:	// メンバ変数
	std::vector<std::unique_ptr<PauseButton>> buttons_;
	int selectedIndex_ = 0;
	bool decided_ = false;

	// 背景用 Sprite
	std::unique_ptr<Sprite> blackBackground_;
};