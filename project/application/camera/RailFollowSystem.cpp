#include "RailFollowSystem.h"
#include "TimeManager.h"
#include "MyTools.h"

using namespace MyBase;

/// 初期化
void RailFollowSystem::Initialize(RailCamera* rail, FollowCamera* follow) {
	// メンバ変数の設定
	railCamera_ = rail;
	followCamera_ = follow;
}

/// 更新
void RailFollowSystem::Update() {
	// レール更新
	railCamera_->Update(TimeManager::GetInstance()->GetDeltaTime());

	// レール終了判定
	if (railCamera_->IsFinished()) isFinished_ = true;

	// レール情報取得
	RailData rail = railCamera_->GetRailData();

	// レール基準座標系構築
	Vector3 worldUp = { 0.0f, 1.0f, 0.0f };

	if (abs(MyTools::Dot(railCamera_->GetRailDirection(), worldUp)) > 0.99f) {
		worldUp = { 0.0f, 0.0f, 1.0f };
	}

	Vector3 right = MyTools::Normalize(MyTools::Cross(worldUp, rail.forward));
	Vector3 up = MyTools::Normalize(MyTools::Cross(rail.forward, right));

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

	Vector3 forward = rail.forward;

	// yaw (左右)
	float yaw = atan2(forward.x, forward.z);

	// pitch (上下)
	float horizontalLength = sqrt(forward.x * forward.x + forward.z * forward.z);
	float pitch = -atan2(forward.y, horizontalLength);

	// roll (とりあえず0)
	float roll = 0.0f;

	// プレイヤーの回転を設定
	playerRotate_ = { pitch, yaw, roll };

	// FollowCamera にオフセットの基準座標系を渡す
	followCamera_->SetBasis(right, up, forward);

	// FollowCamera に追従対象を渡す
	followCamera_->SetTargetPosition(playerPos_);

	// 出力カメラ更新
	followCamera_->Update(TimeManager::GetInstance()->GetDeltaTime());
}