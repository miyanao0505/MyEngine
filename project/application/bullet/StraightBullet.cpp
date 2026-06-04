#include "StraightBullet.h"
#include "MyTools.h"

/// コンストラクタ	
StraightBullet::StraightBullet(BulletOwner owner) : BaseBullet() {
	owner_ = owner;
}

/// 初期化
void StraightBullet::Initialize(const MyBase::Vector3& position, const MyBase::Vector3& direction) {
	// 弾の初期位置を設定
	object_->SetTranslate(position);
	
	// 弾の移動方向を設定
	direction_ = direction;
}

/// 更新
void StraightBullet::Update() {
	// 弾の位置を移動方向に速度分だけ移動させる
	MyBase::Vector3 currentPosition = object_->GetTranslate();

	// 弾の位置を移動方向に速度分だけ移動させる
	currentPosition = MyTools::Add(currentPosition, MyTools::Multiply(speed_, direction_));
	
	// 移動後の位置をオブジェクトに設定
	object_->SetTranslate(currentPosition);
}
