#include "EnemyBullet.h"
#include "ModelManager.h"
#include "MyTools.h"
#include "BaseObjectCollider.h"
#include "CollisionConfig.h"
#include "imgui.h"

using namespace std;

#pragma region 定数
const MyBase::Vector3 EnemyBullet::kInitialScale = {0.5f, 0.5f, 0.5f};
const float EnemyBullet::kColliderRadius = 0.50f;
#pragma endregion

// 初期化
void EnemyBullet::Initialize(const MyBase::Vector3& position, const MyBase::Vector3& velocity)
{
	// ベースオブジェクトの初期化
	BaseObject::Initialize("debug/sphere", "sphere.obj");
	object_->SetTexture("playerBullet.png");
	object_->SetTranslate(position);
	object_->SetScale(kInitialScale);
	velocity_ = velocity;
	// 敵弾のコライダーの初期化
	auto col = make_unique<BaseObjectCollider>(this);
	col->SetRadius(kColliderRadius); // 半径0.50fの球体コライダー
	col->SetTypeId(static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet)); // 敵弾
	SetCollider(std::move(col)); // コライダーをセット
	deathTimer_ = kLifeTime;
}

// 更新
void EnemyBullet::Update(float deltaTime)
{
	// 移動処理
	Move(deltaTime);
	// デスタイマー更新
	deathTimer_ -= deltaTime;
	if (deathTimer_ <= 0.0f)
	{
		isDead_ = true;
	}
	// ベースオブジェクトの更新
	BaseObject::Update();
}

// 描画
void EnemyBullet::Draw()
{
	// ベースオブジェクトの描画
	BaseObject::Draw();
}

#ifdef _DEBUG
// デバッグ描画
void EnemyBullet::DebugDraw()
{
	// ベースオブジェクトのデバッグ描画
	BaseObject::DebugDraw();
}
#endif // _DEBUG

// 移動処理
void EnemyBullet::Move(float deltaTime)
{
	// 弾の移動
	MyBase::Vector3 move = MyTools::Multiply(deltaTime * kMoveSpeed, velocity_);
	object_->SetTranslate(MyTools::Add(object_->GetTranslate(), move));
	// 画面外に出たら削除
	if (deathTimer_ <= 0) {
		isDead_ = true;
	}
}

// 当たり判定
void EnemyBullet::OnCollision([[maybe_unused]] Collider* other)
{
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeId();

	// プレイヤー又はプレイヤーの弾に当たったら削除
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer) ||			// プレイヤーの属性
		typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)) {	// プレイヤーの弾の属性
		isDead_ = true;
	}
}