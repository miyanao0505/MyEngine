#include "StraightBullet.h"
#include "BaseObjectCollider.h"
#include "CollisionConfig.h"
#include "TimeManager.h"
#include "MyTools.h"

using namespace std;

/// コンストラクタ	
StraightBullet::StraightBullet(BulletOwner owner) : BaseBullet() {
	owner_ = owner;
}

/// 初期化
void StraightBullet::Initialize(const MyBase::Vector3& position, const MyBase::Vector3& direction) {
	// ベースオブジェクトの初期化
	BaseObject::Initialize("debug/sphere", "sphere.obj");

	object_->SetTexture("playerBullet.png");
	object_->SetTranslate(position);
	object_->SetScale({ 0.5f, 0.5f, 0.5f });

	// プレイヤー弾のコライダーの初期化
	auto col = make_unique<BaseObjectCollider>(this);
	col->SetRadius(0.50f); // 球体コライダー
	col->SetTypeId(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)); // プレイヤー弾
	SetCollider(std::move(col)); // コライダーをセット
	
	// 弾の移動方向を設定
	direction_ = direction;
}

/// 更新
void StraightBullet::Update() {
	if (isDead_) return; // 弾が消滅している場合は更新しない

	// 移動距離 = 方向ベクトル * 速度 * 経過時間
	MyBase::Vector3 move = MyTools::Multiply(TimeManager::GetInstance()->GetDeltaTime() * speed_, direction_);

	// 現在の位置を取得
	MyBase::Vector3 currentPosition = object_->GetTranslate();

	// 移動後の位置を計算
	currentPosition = MyTools::Add(currentPosition, move);
	
	// 移動後の位置をオブジェクトに設定
	object_->SetTranslate(currentPosition);

	// オブジェクトの更新
	object_->Update();
}

/// 描画
void StraightBullet::Draw() {
	if (isDead_) return; // 弾が消滅している場合は描画しない
	object_->Draw();
}

/// 衝突判定処理
void StraightBullet::OnCollision(Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeId();

	// プレイヤー弾が敵に当たったら削除
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy) ||			// 敵の属性
		typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet)) {	// 敵弾の属性
		isDead_ = true;
	}
}