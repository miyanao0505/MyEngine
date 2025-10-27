#include "GameOverLogo.h"

/// 初期化
void GameOverLogo::Initialize() {
	// 3Dオブジェクト
	// タイトル1文字目(進)
	gameOverChar_ = std::make_unique<BaseObject>();
	gameOverChar_->Initialize("characters", "gameover.obj");
	gameOverChar_->SetName("gameOverChar");
	MyBase::Transform transform = { { 4.0f,4.0f,4.0f },{ 0.0f,0.0f,0.0f },{ 0.0f,3.50f,0.0f } };
	gameOverChar_->GetObject3d()->SetTransform(transform);
}

void GameOverLogo::Finalize() {
	gameOverChar_.reset();
}

void GameOverLogo::Update() {
	if (gameOverChar_) {
		gameOverChar_->Update();
	}
}


void GameOverLogo::Draw() {
	if (gameOverChar_) {
		gameOverChar_->Draw();
	}
}