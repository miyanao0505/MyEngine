#include "GameOverLogo.h"

/// 初期化
void GameOverLogo::Initialize() {
	// 3Dオブジェクト
	// GameOver
	gameOverChar_ = std::make_unique<BaseObject>();
	gameOverChar_->Initialize("characters", "gameover.obj");
	gameOverChar_->SetName("gameOverChar");
	MyBase::Transform transform = { { 4.0f,4.0f,4.0f },{ 0.0f,0.0f,0.0f },{ 0.0f,3.50f,0.0f } };
	gameOverChar_->GetObject3d()->SetTransform(transform);
	// 切り替えボタン
	transitionButton_ = std::make_unique<BaseObject>();
	transitionButton_->Initialize("characters", "enter.obj");
	transitionButton_->SetName("transitionButton");
	transform = { {2.5f, 2.5f, 2.5f}, {0.0f, 0.0f, 0.0f}, {0.0f,-3.0f,0.0f} };
	transitionButton_->GetObject3d()->SetTransform(transform);
}

void GameOverLogo::Finalize() {
	transitionButton_.reset();
	gameOverChar_.reset();
}

void GameOverLogo::Update() {
	if (gameOverChar_) {
		gameOverChar_->Update();
	}
	if (transitionButton_) {
		transitionButton_->Update();
	}
}


void GameOverLogo::Draw() {
	if (gameOverChar_) {
		gameOverChar_->Draw();
	}
	if (transitionButton_) {
		transitionButton_->Draw();
	}
}