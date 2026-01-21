#include "RailFollowSystem.h"
#include "TimeManager.h"
#include "MyTools.h"

/// 初期化
void RailFollowSystem::Initialize(RailCamera* rail, FollowCamera* follow)
{
	// メンバ変数の設定
	railCamera_ = rail;
	followCamera_ = follow;
}

/// 更新
void RailFollowSystem::Update()
{
	// レール更新
	railCamera_->Update(TimeManager::GetInstance()->GetDeltaTime());

	// レール終了判定
	if (railCamera_->GetRailLerpT() >= 1.0f) {
		isFinished_ = true;
	}

	// レール情報取得
	MyBase::RailData rail = railCamera_->GetRailData();

	// レール基準座標系構築
	const MyBase::Vector3 worldUp = { 0.0f, 1.0f, 0.0f };

	MyBase::Vector3 right = MyTools::Normalize(MyTools::Cross(worldUp, rail.forward));
	MyBase::Vector3 up = MyTools::Normalize(MyTools::Cross(rail.forward, right));

	// 入力
	offsetX_ = MyTools::Add(MyTools::Multiply(input_.x * playerSpeed_ * TimeManager::GetInstance()->GetDeltaTime(), right), offsetX_);
	offsetY_ = MyTools::Add(MyTools::Multiply(input_.y * playerSpeed_ * TimeManager::GetInstance()->GetDeltaTime(), up), offsetY_);
	
	// オフセット制限
	if (MyTools::Length(offsetX_) > maxOffsetX_) {
		offsetX_ = MyTools::Multiply(maxOffsetX_, MyTools::Normalize(offsetX_));
	}
	if (MyTools::Length(offsetY_) > maxOffsetY_) {
		offsetY_ = MyTools::Multiply(maxOffsetY_, MyTools::Normalize(offsetY_));
	}

	// 合成オフセット
	offset_ = MyTools::Add(offsetX_, offsetY_);

	// 最終座標
	playerPos_ = MyTools::Add(rail.position, offset_);

	// FollowCamera に追従対象を渡す
	followCamera_->SetTargetPosition(playerPos_);

	// 出力カメラ更新
	followCamera_->Update(TimeManager::GetInstance()->GetDeltaTime());
}