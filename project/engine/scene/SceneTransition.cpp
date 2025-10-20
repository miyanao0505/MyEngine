#include "SceneTransition.h"
#include "TextureManager.h"
#include "CameraManager.h"

void SceneTransition::Start(std::function<void()> onChange)
{
	onChange_ = onChange;
	alpha_ = 0.0f;
	state_ = State::FadeOut;
	isActive_ = true;

	// フェード用スプライトを作成
	fadeSprite_ = std::make_unique<Sprite>();
	fadeSprite_->Initialize(blackbackFilePath_);
	fadeSprite_->SetPosition({ 0.0f, 0.0f });
	fadeSprite_->SetSize({ 1280.0f, 720.0f }); // 画面全体を覆うサイズにする
	fadeSprite_->SetAnchorPoint({ 0.0f, 0.0f }); // 左上を基準点にする
	fadeSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 最初は透明
}

void SceneTransition::Update()
{
	switch (state_)
	{
	case State::FadeOut:
		alpha_ += speed_;
		fadeSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ });
		fadeSprite_->Update();
		if (alpha_ >= 1.0f) {
			alpha_ = 1.0f;
			state_ = State::ChangeScene;
		}
		break;

	case State::ChangeScene:
		if (onChange_) {
			onChange_();
			onChange_ = nullptr;
		}
		state_ = State::FadeIn;
		fadeSprite_->Update();
		break;

	case State::FadeIn:
		alpha_ -= speed_;
		fadeSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ });
		fadeSprite_->Update();
		if (alpha_ <= 0.0f) {
			alpha_ = 0.0f;
			state_ = State::None;
			isActive_ = false;
		}
		break;

	default:
		break;
	}
}

void SceneTransition::Draw()
{
#pragma region スプライト

	// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	TextureManager::GetInstance()->SetCommonScreen();

	// 全てのSprite個々の描画
	// 画面全体に黒い四角形を描画
	fadeSprite_->Draw();

#pragma endregion スプライト
}