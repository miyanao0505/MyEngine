#include "ClearLogo.h"
#include "MyTools.h"
#include "TimeManager.h"

#pragma region 定数
const MyBase::Transform ClearLogo::kCharTransform{ { 4.0f,4.0f,4.0f },{ 0.0f,0.0f,0.0f },{ 0.0f,3.50f,0.0f } };
const MyBase::Transform ClearLogo::kButtonTransform{ {2.5f, 2.5f, 2.5f}, {0.0f, 0.0f, 0.0f}, {0.0f,-3.0f,0.0f} };

const MyBase::Vector3 ClearLogo::kMoveVector = { 0.0f,-1.0f,0.0f };
const float ClearLogo::kMoveDistance = 0.0f;
const float ClearLogo::kMoveSpeed = 60.0f;
#pragma endregion

/// 初期化
void ClearLogo::Initialize() {
	// 3Dオブジェクト
	// Clear
	clearChar_ = std::make_unique<BaseObject>();
	clearChar_->Initialize("characters", "clear.obj");
	clearChar_->SetName("clearChar");
	MyBase::Transform transform = kCharTransform;
	clearChar_->GetObject3D()->SetTransform(transform);
	// 切り替えボタン
	transitionButton_ = std::make_unique<BaseObject>();
	transitionButton_->Initialize("characters", "enter.obj");
	transitionButton_->SetName("transitionButton");
	transform = kButtonTransform;
	transitionButton_->GetObject3D()->SetTransform(transform);

	moveVector_ = kMoveVector;
	moveDistance_ = kMoveDistance;
}

/// 終了
void ClearLogo::Finalize() {
	transitionButton_.reset();
	clearChar_.reset();
}

/// 更新
void ClearLogo::Update(float deltaTime) {
	Move(deltaTime);

	if (clearChar_) {
		clearChar_->Update(deltaTime);
	}
	if (transitionButton_) {
		transitionButton_->Update(deltaTime);
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
void ClearLogo::Move(float deltaTime)
{
	if (moveDistance_ <= -kMaxMoveDistance) {
		moveVector_ = MyTools::Multiply(kReverse, moveVector_);
	}
	else if (moveDistance_ >= kMaxMoveDistance) {
		moveVector_ = MyTools::Multiply(kReverse, moveVector_);
	}

	MyBase::Vector3 move = MyTools::Multiply(kMoveSpeed * deltaTime, moveVector_);
	moveDistance_ += move.y;

	// 移動処理
	clearChar_->GetObject3D()->SetTranslate(MyTools::Add(clearChar_->GetObject3D()->GetTranslate(), move));
}
