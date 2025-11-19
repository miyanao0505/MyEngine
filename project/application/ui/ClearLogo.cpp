#include "ClearLogo.h"
#include "MyTools.h"

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

	moveVector_ = { 0.0f,-1.0f,0.0f };
	moveDistance_ = 0.0f;
}

/// 終了
void ClearLogo::Finalize() {
	transitionButton_.reset();
	clearChar_.reset();
}

/// 更新
void ClearLogo::Update() {
	Move();

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

/// ロゴの移動処理
void ClearLogo::Move()
{
	if (moveDistance_ <= -moveSpeed_ * 20.0f) {
		//moveVector_ = MyTools::Multiply(-1.0f, moveVector_);
		moveVector_ = { 0.0f,1.0f,0.0f };
	}
	else if (moveDistance_ >= moveSpeed_ * 20.0f) {
		//moveVector_ = MyTools::Multiply(-1.0f, moveVector_);
		moveVector_ = { 0.0f,-1.0f,0.0f };
	}

	MyBase::Vector3 move = MyTools::Multiply(moveSpeed_, moveVector_);
	moveDistance_ += move.y;

	// 移動処理
	clearChar_->GetObject3d()->SetTranslate(MyTools::Add(clearChar_->GetObject3d()->GetTranslate(), move));
}
