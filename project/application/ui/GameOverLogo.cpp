#include "GameOverLogo.h"
#include "MyBase.h"

#pragma region 定数
const MyBase::Transform GameOverLogo::kCharTransform{ { 4.0f,4.0f,4.0f },{ 0.0f,0.0f,0.0f },{ 0.0f,3.50f,0.0f } };
const MyBase::Transform GameOverLogo::kButtonTransform{ {2.5f, 2.5f, 2.5f}, {0.0f, 0.0f, 0.0f}, {0.0f,-3.0f,0.0f} };
#pragma endregion

/// 初期化
void GameOverLogo::Initialize() {
	// 3Dオブジェクト
	// GameOver
	gameOverChar_ = std::make_unique<BaseObject>();
	gameOverChar_->Initialize("characters", "gameover.obj");
	gameOverChar_->SetName("gameOverChar");
	MyBase::Transform transform = kCharTransform;
	gameOverChar_->GetObject3D()->SetTransform(transform);
	// 切り替えボタン
	transitionButton_ = std::make_unique<BaseObject>();
	transitionButton_->Initialize("characters", "enter.obj");
	transitionButton_->SetName("transitionButton");
	transform = kButtonTransform;
	transitionButton_->GetObject3D()->SetTransform(transform);
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