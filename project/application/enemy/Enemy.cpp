#include "Enemy.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "BaseObjectCollider.h"
#include "CollisionConfig.h"
#include "Player.h"
#include "MyTools.h"
#include <imgui.h>
#include <cmath>

using namespace std;

Enemy::~Enemy()
{

}

// 初期化
void Enemy::Initialize()
{
	// ベースオブジェクトの初期化
	BaseObject::Initialize("enemy", "enemy.obj");
	
	// 3Dオブジェクトの初期化
	object_->SetTranslate({ 10.0f, 0.0f, 150.0f });	// 初期位置
	object_->SetScale({ 1.0f, 1.0f, 1.0f });		// 初期スケール
	object_->SetRotate({ 0.0f, 3.14f, 0.0f});	// 初期回転

	// 敵のコライダーの初期化
	auto col = make_unique<BaseObjectCollider>(this);
	col->SetRadius(1.50f); // 半径1.50fの球体コライダー
	col->SetAABB({ { 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f} });
	col->SetOBB({ { 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} });
	col->SetTypeId(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)); // コリジョン属性
	SetCollider(std::move(col)); // コライダーをセット
	
	// パーティクルエミッターの初期化
	particleEmitter_ = std::make_unique<ParticleEmitter>();
	particleEmitter_->Initialize("hitEffectEnemy", "circle.png", ParticleType::kEllipse);
	particleEmitter_->SetPosition(object_->GetTranslate());
	particleEmitter_->SetSize({ 1.0f, 1.0f, 1.0f }); // 初期サイズ
	ParticleSystem::ParticleGroupData hitEffect = {
		.size = { 1.f, 10.0f },
		.energy = { 1.0f, 1.0f },
		.count = { 10, 15 },
		.speed = { 0.0f, 0.0f },
		.direction = { 0.0f, 0.0f, 0.0f },
		.color = { 0.88f, 0.28f, 0.0f, 1.0f },
		.frequency = 1.5f,
		.isBillboard = true,
		.isEmitUpdate = true
	};
	particleEmitter_->SetParticleGroupData("hitEffectEnemy", hitEffect);
	particleEmitter_->CreateParticleGroup("hitEffectRingEnemy", "gradationLine.png", ParticleType::kRing);
	particleEmitter_->SetPosition(object_->GetTranslate());
	particleEmitter_->SetSize({ 1.0f, 1.0f, 1.0f }); // 初期サイズ
	ParticleSystem::ParticleGroupData hitEffectRing = {
		.size = { 1.75f, 1.75f },
		.energy = { 1.0f, 1.0f },
		.count = { 1, 5 },
		.speed = { 0.0f, 0.0f },
		.direction = { 0.0f, 0.0f, 0.0f },
		.color = { 0.88f, 0.28f, 0.0f, 1.0f },
		.frequency = 1.5f,
		.isBillboard = true,
		.isEmitUpdate = true
	};
	particleEmitter_->SetParticleGroupData("hitEffectRingEnemy", hitEffectRing);

	// 弾の初期化
	bullets_.clear();

	// 敵のステータスの初期化
	hp_ = 50; // 初期HP
	isDead_ = false; // 初期状態は生存
	attackPower_ = 10;
	attackCoolTime_ = kAttackCoolTime;
}

// 更新
void Enemy::Update(float deltaTime)
{
	// 敵の更新処理
	if (isDead_) {
		return; // 死んでいる場合は更新しない
	}

	Attack();

	// 弾更新
	for (auto it = bullets_.begin(); it != bullets_.end(); ) {
		const bool isDead = (*it)->IsDead();
		const float distance = MyTools::Length(MyTools::Subtract((*it)->GetWorldPosition(), GetWorldPosition()));

		if (isDead || distance >= kBulletDrawDistance) {
			it = bullets_.erase(it); // listから完全に削除
		}
		else {
			it->get()->Update(deltaTime); // 弾の更新
			++it;
		}
	}

	// 攻撃のクールタイムを減らす
	if (attackCoolTime_ > 0) {
		attackCoolTime_ -= deltaTime;
	}

	// ダメージリアクションの更新
	if(damageReactionTimer_ > 0.0f){
		// ダメージリアクションタイマーの更新
		damageReactionTimer_ -= deltaTime;
		DamageReactionUpdate();
	}

	if (state_) {
		state_->Update();
	}

	// モデルの更新
	object_->Update();
}

// 描画
void Enemy::Draw()
{
	// 敵の描画処理
	if (isDead_) {
		return; // 死んでいる場合は描画しない
	}

	// モデルの描画
	object_->Draw();

	// 弾の描画
	for (auto it = bullets_.begin(); it != bullets_.end(); ) {
		it->get()->Draw();
		++it;
	}
}

// Updateのステートチェンジ
void Enemy::ChangeState(std::unique_ptr<EnemyBaseState> state)
{
	state_ = std::move(state);
	state_->Initialize();
}

// ダメージ処理
void Enemy::Damage(int damage)
{
	if(damageReactionTimer_ > 0.0f){
		return; // ダメージリアクション中はダメージを受け付けない
	}

	// HPを減少
	hp_ -= damage;
	
	if (hp_ > 0.0f) {
		// ダメージリアクション開始
		DamageReactionStart();
	}
}

// ダメージリアクション開始
void Enemy::DamageReactionStart()
{
	// ダメージリアクションタイマーをリセット
	damageReactionTimer_ = kDamageReactionDuration;

	// パーティクルを発生させる
	particleEmitter_->Emit();

	// 赤を強調
	object_->GetModel()->GetModelMaterial()->color = { 1.0f, 0.5f, 0.5f, 1.0f };
}

// ダメージリアクション
void Enemy::DamageReactionUpdate()
{
	// ダメージリアクション終了確認
	if (damageReactionTimer_ <= 0.0f) {
		// タイマーリセットと色リセット
		damageReactionTimer_ = 0.0f;
		object_->GetModel()->GetModelMaterial()->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		return;
	}

	return;
}

// 死亡リアクション
void Enemy::DeadReaction()
{

}

#ifdef _DEBUG
// デバッグ描画
void Enemy::DebugDraw()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Enemy")) {
		// ステータス表示
		ImGui::Text("HP: %d\n", hp_);
		ImGui::Text("Attack Power: %d\n", attackPower_);

		MyBase::Transform transform = { object_->GetScale(), object_->GetRotate(), object_->GetTranslate() };

		// 移動
		ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f, -100.0f, 100.0f);
		// 回転
		ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f, -3.14f, 3.14f);
		// 拡縮
		ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f, 0.01f, 10.0f);
		object_->SetTransform(transform);

		ImGui::Text("\n");

		// パーティクルエミッターのデバッグUI
		if (particleEmitter_) {
			particleEmitter_->ImGui("Enemy");
		}
	}
	ImGui::PopID();
}
#endif // _DEBUG

void Enemy::Attack()
{
	// 攻撃不可なら早期リターン
	if (!CanAttack()) return;
	// 弾の生成
	SpawnBullet();
}

bool Enemy::CanAttack()
{
	if (bullets_.size() < kMaxBulletCount && attackCoolTime_ <= 0) {
		return true;
	}
	return false;
}

void Enemy::SpawnBullet()
{
	auto bullet = std::make_unique<EnemyBullet>();
	MyBase::Vector3 direction = Matrix::TransformNormal({ 0.0f, 0.0f, -1.0f }, object_->GetWorldTransform()->GetWorldMatrix());
	bullet->Initialize(MyTools::Add(object_->GetTranslate(), direction), MyTools::Normalize(direction));
	bullet->SetAttackPower(attackPower_);
	bullets_.emplace_back(std::move(bullet));
	// 攻撃のクールタイムを設定
	attackCoolTime_ = kAttackCoolTime;
}

// 当たり判定
void Enemy::OnCollision([[maybe_unused]] Collider* other)
{
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeId();

	// プレイヤー又はプレイヤー弾が当たった時の処理
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer) ||			// プレイヤーの属性
		typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)) {	// プレイヤー弾の属性
		// プレイヤーの攻撃力分ダメージを受ける
		Damage(player_->GetAttackPower());
		if (hp_ <= 0) {
			isDead_ = true;
		}
	}
}
