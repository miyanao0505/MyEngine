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
public:	// enum
	enum class State {
		None,       // 非アクティブ
		FadeOut,    // フェードアウト中
		ChangeScene,// シーン切替
		FadeIn      // フェードイン中
	};

public:	// メンバ関数
	/// <summary>
	/// フェード演出の開始処理
	/// </summary>
	/// <param name="onChange">フェードアウト完了後に実行されるシーン切替コールバック関数</param>
	void Start(std::function<void()> onChange);

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
	bool IsActive() const { return isActive_; }
	
	/// <summary>
	/// 演出が終了したかどうかを取得
	/// </summary>
	/// <returns>完了していれば true、未完了なら false</returns>
	bool IsFinished() const { return !isActive_; }

private:	// メンバ変数
	State state_ = State::None;
	float alpha_ = 0.0f;        // フェード量 (0〜1)
	float speed_ = 0.02f;       // フェード速度
	bool isActive_ = false;     // 演出中フラグ
	std::function<void()> onChange_; // シーン切替タイミングのコールバック

	// テクスチャファイルパス
	const std::string blackbackFilePath_ = "resources/texture/blackback.png";

	// フェード用スプライト
	std::unique_ptr<Sprite> fadeSprite_ = nullptr;
};

