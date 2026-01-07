#include "RailFollowSystem.h"
#include "TimeManager.h"
#include "MyTools.h"

/// 初期化
void RailFollowSystem::Initialize(RailCamera* rail, FollowCamera* follow, Camera* outCamera)
{
	// メンバ変数の設定
	railCamera_ = rail;
	followCamera_ = follow;
	camera_ = outCamera;
}

/// 更新
void RailFollowSystem::Update()
{
	// レール更新
	railCamera_->Update(TimeManager::GetInstance()->GetDeltaTime());

	// レール情報取得
	MyBase::RailData rail = railCamera_->GetRailData();

	// レール基準座標系構築
	const MyBase::Vector3 worldUp = { 0.0f, 1.0f, 0.0f };

	MyBase::Vector3 right = MyTools::Normalize(MyTools::Cross(worldUp, rail.forward));
	MyBase::Vector3 up = MyTools::Normalize(MyTools::Cross(rail.forward, right));

	// 入力
	MyBase::Vector3 offset = MyTools::Add(MyTools::Multiply(maxOffsetX_, MyTools::Multiply(input_.x, right)), MyTools::Multiply(maxOffsetY_, MyTools::Multiply(input_.y, up)));

	// 最終座標
	playerPos_ = MyTools::Add(rail.position, offset);

	// FollowCamera に追従対象を渡す
	followCamera_->SetTargetPosition(playerPos_);

	// 出力カメラ更新
	followCamera_->Update(TimeManager::GetInstance()->GetDeltaTime());
}