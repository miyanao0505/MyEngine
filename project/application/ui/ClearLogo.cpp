#include "ClearLogo.h"
#include "MyBase.h"

/// 初期化
void ClearLogo::Initialize() {
	// 3Dオブジェクト
	// Clear
	clearChar_ = std::make_unique<BaseObject>();
	clearChar_->Initialize("characters", "clear.obj");
	clearChar_->SetName("clearChar");
	MyBase::Transform transform = { { 4.0f,4.0f,4.0f },{ 0.0f,0.0f,0.0f },{ 0.0f,3.50f,0.0f } };
	clearChar_->GetObject3d()->SetTransform(transform);
	// 切り替えボタン
	transitionButton_ = std::make_unique<BaseObject>();
	transitionButton_->Initialize("characters", "enter.obj");
	transitionButton_->SetName("transitionButton");
	transform = { {2.5f, 2.5f, 2.5f}, {0.0f, 0.0f, 0.0f}, {0.0f,-3.0f,0.0f} };
	transitionButton_->GetObject3d()->SetTransform(transform);
}

/// 終了
void ClearLogo::Finalize() {
	transitionButton_.reset();
	clearChar_.reset();
}

/// 更新
void ClearLogo::Update() {
	if (clearChar_) {
		clearChar_->Update();
	}
	if (transitionButton_) {
		transitionButton_->Update();
	}
}

/// 描画
void ClearLogo::Draw() {
	if (clearChar_) {
		clearChar_->Draw();
	}
	if (transitionButton_) {
		transitionButton_->Draw();
	}
}