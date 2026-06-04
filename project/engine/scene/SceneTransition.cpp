#include "SceneTransition.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include <algorithm>

void SceneTransition::Start(FinishCallback onSwitch)
{
	phase_ = Phase::kFadeIn;
	timer_ = 0.0f;
	onSwitch_ = std::move(onSwitch);

	// フェード用スプライトを作成
	fadeSprite_ = std::make_unique<Sprite>();
	fadeSprite_->Initialize("white1x1.png");
	fadeSprite_->SetPosition({ 0.0f, 0.0f });
	fadeSprite_->SetSize(kScreenSize);					// 画面全体を覆うサイズにする
	fadeSprite_->SetAnchorPoint({ 0.0f, 0.0f });		// 左上を基準点にする
	fadeSprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });	// 最初は透明
}

void SceneTransition::Update()
{
	if (phase_ == Phase::kNone) return;

	timer_ += kDeltaTime;

	if (timer_ >= kFadeTime) {
		timer_ = 0.0f;

		if (phase_ == Phase::kFadeIn) {
			// ▼ シーン差し替えタイミング
			if (onSwitch_) {
				onSwitch_();
			}
			phase_ = Phase::kFadeOut;
		}
		else if (phase_ == Phase::kFadeOut) {
			phase_ = Phase::kNone;
			onSwitch_ = nullptr;
		}
	}

	// フェードの進行に応じてアルファ値を変更
	float alpha = timer_ / kFadeTime;
	if (phase_ == Phase::kFadeOut) {
		alpha = 1.0f - alpha; // フェードアウト時は逆転
	}
	fadeSprite_->SetColor({ 0.0f, 0.0f, 0.0f, alpha });
	fadeSprite_->Update();
}

void SceneTransition::Draw()
{
	if (phase_ == Phase::kNone) return;

	// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	TextureManager::GetInstance()->SetCommonScreen();

	// 全てのSprite個々の描画
	// 画面全体に黒い四角形を描画
	fadeSprite_->Draw();
}