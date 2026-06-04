#include "BaseBullet.h"
#include "TimeManager.h"

/// 更新
void BaseBullet::Update() {
	// 寿命を減らす
    	deathTimer_ -= TimeManager::GetInstance()->GetDeltaTime();

	// 寿命が尽きたら消滅フラグを立てる
	if (deathTimer_ <= 0.0f) {
		isDead_ = true;
	}

	object_->Update();
}

/// 描画
void BaseBullet::Draw() {
	object_->Draw();
}