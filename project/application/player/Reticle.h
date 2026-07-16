#pragma once
#include "Sprite.h"
#include <memory>

/// <summary>
/// レティクルクラス
/// </summary>
class Reticle {
private:
#pragma region 定数
	static constexpr float kDistance3DReticle = 150.0f;
#pragma endregion
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:	// getter
	/// <summary>
	/// スクリーン座標を取得
	/// </summary>
	/// <returns>スクリーン座標</returns>
	const MyBase::Vector2& GetScreenPosition() const { return screenPosition_; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	const MyBase::Vector3& GetWorldPosition() const { return worldPosition_; }

	/// <summary>
	/// 表示状態を取得
	/// </summary>
	/// <returns>表示状態</returns>
	bool isVisible() const { return isVisible_; }

public:	// setter
	/// <summary>
	/// 表示状態を設定
	/// </summary>
	/// <param name="visible">表示状態</param>
	void SetVisible(bool visible) { isVisible_ = visible; }

private:	// メンバ関数
	/// <summary>
	/// ワールド座標をスクリーン座標に変換
	/// </summary>
	void UpdateWorldPosition();

private:	// メンバ変数
	std::unique_ptr<Sprite> sprite_ = nullptr;

	MyBase::Vector2 screenPosition_;

	MyBase::Vector3 worldPosition_;

	bool isVisible_ = true;
};