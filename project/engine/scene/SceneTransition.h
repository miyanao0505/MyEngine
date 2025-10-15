#pragma once
#include <functional>
#include <string>
#include <memory>
#include "Sprite.h"

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
	void Start(std::function<void()> onChange);
	void Update();
	void Draw();    // αを使って黒フェードを描画
	bool IsActive() const { return isActive_; }
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

