#pragma once
#include "Sprite.h"
#include "MyBase.h"
#include <memory>

/// <summary>
/// レティクルクラス
/// </summary>
class Reticle {
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
	/// 
	/// </summary>
	/// <returns></returns>
	const Vector2& GetScreenPosition() const { return screenPosition_; }

private:	// メンバ変数
	Sprite* sprite_ = nullptr;

	Vector2 screenPosition_;

	bool isVisible_ = true;
};