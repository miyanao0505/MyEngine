#pragma once
#include <string>
#include <memory>
#include "Sprite.h"
#include "PauseButtonDesc.h"

/// <summary>
/// ポーズ画面用ボタンクラス
/// </summary>
class PauseButton
{
private:
#pragma region 定数
	static constexpr MyBase::Vector2 kButtonSize{ 400.0f, 100.0f };
	static constexpr float kSelectMagnification = 1.2f;

	static constexpr MyBase::Vector2 kAnchorPoint{ 0.5f, 0.5f };
#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="desc">ボタン情報</param>
	/// <param name="label">ボタン名</param>
	PauseButton(const PauseButtonDesc desc, const std::string& label);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="isSelected">選択してるか</param>
	void Update(bool isSelected);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// ボタンのサイズ更新
	/// </summary>
	/// <param name="size">サイズ</param>
	void ButtonSizeUpdate(MyBase::Vector2 size);

public:	// getter
	/// <summary>
	/// ボタンタイプの取得
	/// </summary>
	/// <returns>ボタンタイプ</returns>
	PauseButtonType GetType() const { return type_; }

private:	// メンバ変数
	PauseButtonType type_;
	std::string label_;

	std::unique_ptr<Sprite> sprite_ = nullptr;
};