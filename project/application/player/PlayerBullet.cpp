#include "PlayerBullet.h"
#include "ModelManager.h"
#include "MyTools.h"
#include "BaseObjectCollider.h"
#include "CollisionConfig.h"
#include "imgui.h"

using namespace std;

// 初期化
void PlayerBullet::Initialize(const MyBase::Vector3& position, const MyBase::Vector3& velocity)
{
	// ベースオブジェクトの初期化
	BaseObject::Initialize("debug/sphere", "sphere.obj");

	object_->SetTexture("resources/texture/playerBullet.png");
	object_->SetTranslate(position);
	object_->SetScale({ 0.5f, 0.5f, 0.5f });

	velocity_ = velocity;

	// プレイヤー弾のコライダーの初期化
	auto col = make_unique<BaseObjectCollider>(this);
	col->SetRadius(0.50f); // 半径0.50fの球体コライダー
	col->SetAABB({ { 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f} });
	col->SetOBB({ { 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} });
	col->SetTypeId(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)); // プレイヤー弾
	SetCollider(std::move(col)); // コライダーをセット

	deathTimer_ = kLifeTime;
}

// 更新
void PlayerBullet::Update()
{
	// 移動処理
	Move();
	// オブジェクトの更新
	object_->Update();

	// 寿命を減らす
	deathTimer_--;
}

// 描画
void PlayerBullet::Draw()
{
	// プレイヤー弾
	if (isDead_) {
		return; // 弾が消滅している場合は描画しない
	}
	object_->Draw();
}

#ifdef _DEBUG
// デバック描画
void PlayerBullet::DebugDraw()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("PlayerBullet")) {
		MyBase::Transform transform = { object_->GetScale(), object_->GetRotate(), object_->GetTranslate() };

		// 移動
		ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f, -100.0f, 100.0f);
		// 回転
		ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f, -3.14f, 3.14f);
		// 拡縮
		ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f, 0.01f, 10.0f);
		object_->SetTransform(transform);

		ImGui::Text("\n");
	}
	ImGui::PopID();
}
#endif // _DEBUG

// 移動処理
void PlayerBullet::Move()
{
	// 弾の移動
	MyBase::Vector3 move = MyTools::Multiply(kMoveSpeed, velocity_);
	object_->SetTranslate(MyTools::Add(object_->GetTranslate(), move));
	// 画面外に出たら削除
	if (deathTimer_ <= 0) {
		isDead_ = true;
	}
}

// 当たり判定
void PlayerBullet::OnCollision([[maybe_unused]] Collider* other)
{
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeId();

	// プレイヤー弾が敵に当たったら削除
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) { // 敵の属性
		isDead_ = true;
	}
	
	

}