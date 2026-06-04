#include "BaseBullet.h"

/// 更新
void BaseBullet::Update() {
	object_->Update();
}

/// 描画
void BaseBullet::Draw() {
	object_->Draw();
}