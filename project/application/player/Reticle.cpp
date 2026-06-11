#include "Reticle.h"
#include "Enemy.h"
#include "MyTools.h"

using namespace std;
using namespace MyBase;

#pragma region 定数
const Vector3 Reticle::kInitialScale = { 1.0f, 1.0f, 1.0f };
#pragma endregion

/// 初期化
void Reticle::Initialize() {
	BaseObject::Initialize("reticle", "reticle.obj");

	SetName("Reticle");

	object_->SetScale(kInitialScale);
}

/// 更新
void Reticle::Update() {
	// ロックオン中
	if (target_ != nullptr) {
		position_ = target_->GetWorldPosition();
	}

	// 座標をセット
	object_->SetTranslate(position_);

	// 更新
	object_->Update();
}

/// 描画
void Reticle::Draw() {
	if (!isVisible_) return;

	object_->Draw();
}

#ifdef _DEBUG
/// デバッグ描画
void Reticle::DebugDraw() {

}
#endif // _DEBUG

/// 
void Reticle::FollowPlayer(const Vector3& playerPos, const Vector3& forward) {
	position_ = MyTools::Add(playerPos, MyTools::Multiply(kDistance, forward));
}

/// 
void Reticle::LockOn(Enemy* target) {
	target_ = target;
}

/// 
void Reticle::ReleaseLockOn() {
	target_ = nullptr;
}