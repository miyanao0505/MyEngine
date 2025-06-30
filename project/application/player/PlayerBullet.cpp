#include "PlayerBullet.h"
#include "ModelManager.h"
#include "MyTools.h"
#include "CollisionConfig.h"
#include "imgui.h"

// 初期化
void PlayerBullet::Initialize(MyBase::Vector3 position)
{
	// プレイヤー弾のコライダーの初期化
	SetRadius(0.5f);
	SetSize({ 0.5f, 0.5f });
	SetTypeId(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer)); // プレイヤー弾

	// モデルの初期化
	ModelManager::GetInstance()->LoadModel("resources/model/debug/sphere", "sphere.obj");
	object_ = std::make_unique<Object3d>();
	object_->Initislize("sphere.obj");
	object_->SetTranslate(position);
	object_->SetScale({ 0.5f, 0.5f, 0.5f });
	
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
		
		ImGui::PopID();
	}
}
#endif // _DEBUG

// 移動処理
void PlayerBullet::Move()
{
	// 弾の移動
	MyBase::Vector3 move = { 0.0f, 0.0f, kmoveSpeed_ };
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