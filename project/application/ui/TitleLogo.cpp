#include "TitleLogo.h"
#include "ModelManager.h"
#include "MyTools.h"

// 初期化
void TitleLogo::Initialize() {
	// 3Dオブジェクト
	// タイトル1文字目(進)
	titleCharFirst_ = std::make_unique<BaseObject>();
	titleCharFirst_->Initialize("characters", "title_1.obj");
	titleCharFirst_->SetName("TitleObject1");
	MyBase::Transform transform = { { 5.0f,5.0f,5.0f },{ 0.0f,0.0f,0.0f },{ -5.0f,5.0f,0.0f } };
	titleCharFirst_->GetObject3D()->SetTransform(transform);
	// タイトル2文字目(空)
	titleCharSecond_ = std::make_unique<BaseObject>();
	titleCharSecond_->Initialize("characters", "title_2.obj");
	titleCharSecond_->SetName("TitleObject2");
	transform = { { 5.0f,5.0f,5.0f },{ 0.0f,0.0f,0.0f },{ 0.0f,5.0f,0.0f } };
	titleCharSecond_->GetObject3D()->SetTransform(transform);
	// タイトル3文字目(戦)
	titleCharThird_ = std::make_unique<BaseObject>();
	titleCharThird_->Initialize("characters", "title_3.obj");
	titleCharThird_->SetName("TitleObject3");
	transform = { { 5.0f,5.0f,5.0f },{ 0.0f,0.0f,0.0f },{ 5.0f,5.0f,0.0f } };
	titleCharThird_->GetObject3D()->SetTransform(transform);
	// スタートボタン文字(Enter)
	startButton_ = std::make_unique<BaseObject>();
	startButton_->Initialize("characters", "enter.obj");
	startButton_->SetName("Enter");
	transform = { { 5.0f,5.0f,5.0f },{ 0.0f,0.0f,0.0f },{ 0.0f,-2.50f,0.0f } };
	startButton_->GetObject3D()->SetTransform(transform);

	moveVector_ = { 0.0f,-1.0f,0.0f };
	moveDistance_ = 0.0f;
}

// 終了
void TitleLogo::Finalize() {
	startButton_.reset();
	titleCharThird_.reset();
	titleCharSecond_.reset();
	titleCharFirst_.reset();
}

// 更新
void TitleLogo::Update() {
	Move();

	if (titleCharFirst_) {
		titleCharFirst_->Update();
	}
	if (titleCharSecond_) {
		titleCharSecond_->Update();
	}
	if (titleCharThird_) {
		titleCharThird_->Update();
	}
	if (startButton_) {
		startButton_->Update();
	}
}

// 描画
void TitleLogo::Draw() {
	if (titleCharFirst_) {
		titleCharFirst_->Draw();
	}
	if (titleCharSecond_) {
		titleCharSecond_->Draw();
	}
	if (titleCharThird_) {
		titleCharThird_->Draw();
	}
	if (startButton_) {
		startButton_->Draw();
	}
}

// ロゴの移動処理
void TitleLogo::Move() {
	if (moveDistance_ <= -kMoveSpeed * 30.0f) {
		moveVector_ = { 0.0f,1.0f,0.0f };
	}
	else if (moveDistance_ >= kMoveSpeed * 30.0f) {
		moveVector_ = { 0.0f,-1.0f,0.0f };
	}

	MyBase::Vector3 move = MyTools::Multiply(kMoveSpeed, moveVector_);
	moveDistance_ += move.y;
	
	// 移動処理
	titleCharFirst_->GetObject3D()->SetTranslate(MyTools::Add(titleCharFirst_->GetObject3D()->GetTranslate(), move));
	titleCharSecond_->GetObject3D()->SetTranslate(MyTools::Add(titleCharSecond_->GetObject3D()->GetTranslate(), move));
	titleCharThird_->GetObject3D()->SetTranslate(MyTools::Add(titleCharThird_->GetObject3D()->GetTranslate(), move));
}