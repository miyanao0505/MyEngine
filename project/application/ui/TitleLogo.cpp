#include "TitleLogo.h"
#include "ModelManager.h"
#include "MyTools.h"

#pragma region 定数
const MyBase::Transform TitleLogo::kCharFirstTransform{ { 5.0f,5.0f,5.0f },{ 0.0f,0.0f,0.0f },{ -5.0f,5.0f,0.0f } };
const MyBase::Transform TitleLogo::kCharSecondTransform{ { 5.0f,5.0f,5.0f },{ 0.0f,0.0f,0.0f },{ 0.0f,5.0f,0.0f } };
const MyBase::Transform TitleLogo::kCharThirdTransform{ { 5.0f,5.0f,5.0f },{ 0.0f,0.0f,0.0f },{ 5.0f,5.0f,0.0f } };
const MyBase::Transform TitleLogo::kButtonTransform{ { 5.0f,5.0f,5.0f },{ 0.0f,0.0f,0.0f },{ 0.0f,-2.50f,0.0f } };

const MyBase::Vector3 TitleLogo::kMoveVector{ 0.0f,-1.0f,0.0f };
const float TitleLogo::kMoveDistance = 0.0f;
const float TitleLogo::kMoveSpeed = 5.0f;
#pragma endregion

// 初期化
void TitleLogo::Initialize() {
	// 3Dオブジェクト
	// タイトル1文字目(進)
	titleCharFirst_ = std::make_unique<BaseObject>();
	titleCharFirst_->Initialize("characters", "title_1.obj");
	titleCharFirst_->SetName("TitleObject1");
	MyBase::Transform transform = kCharFirstTransform;
	titleCharFirst_->GetObject3D()->SetTransform(transform);
	// タイトル2文字目(空)
	titleCharSecond_ = std::make_unique<BaseObject>();
	titleCharSecond_->Initialize("characters", "title_2.obj");
	titleCharSecond_->SetName("TitleObject2");
	transform = kCharSecondTransform;
	titleCharSecond_->GetObject3D()->SetTransform(transform);
	// タイトル3文字目(戦)
	titleCharThird_ = std::make_unique<BaseObject>();
	titleCharThird_->Initialize("characters", "title_3.obj");
	titleCharThird_->SetName("TitleObject3");
	transform = kCharThirdTransform;
	titleCharThird_->GetObject3D()->SetTransform(transform);
	// スタートボタン文字(Enter)
	startButton_ = std::make_unique<BaseObject>();
	startButton_->Initialize("characters", "enter.obj");
	startButton_->SetName("Enter");
	transform = kButtonTransform;
	startButton_->GetObject3D()->SetTransform(transform);

	moveVector_ = { 0.0f,-1.0f,0.0f };
	moveDistance_ = kMoveDistance;
}

// 終了
void TitleLogo::Finalize() {
	startButton_.reset();
	titleCharThird_.reset();
	titleCharSecond_.reset();
	titleCharFirst_.reset();
}

// 更新
void TitleLogo::Update(float deltaTime) {
	Move(deltaTime);

	if (titleCharFirst_) {
		titleCharFirst_->Update(deltaTime);
	}
	if (titleCharSecond_) {
		titleCharSecond_->Update(deltaTime);
	}
	if (titleCharThird_) {
		titleCharThird_->Update(deltaTime);
	}
	if (startButton_) {
		startButton_->Update(deltaTime);
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
void TitleLogo::Move(float deltaTime) {
	if (moveDistance_ <= -kMaxMoveDistance) {
		moveVector_ = MyTools::Multiply(kReverse, moveVector_);
	}
	else if (moveDistance_ >= kMaxMoveDistance) {
		moveVector_ = MyTools::Multiply(kReverse, moveVector_);
	}

	MyBase::Vector3 move = MyTools::Multiply(kMoveSpeed * deltaTime, moveVector_);
	moveDistance_ += move.y;
	
	// 移動処理
	titleCharFirst_->GetObject3D()->SetTranslate(MyTools::Add(titleCharFirst_->GetObject3D()->GetTranslate(), move));
	titleCharSecond_->GetObject3D()->SetTranslate(MyTools::Add(titleCharSecond_->GetObject3D()->GetTranslate(), move));
	titleCharThird_->GetObject3D()->SetTranslate(MyTools::Add(titleCharThird_->GetObject3D()->GetTranslate(), move));
}