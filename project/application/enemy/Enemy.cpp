#include "Enemy.h"
#include <imgui.h>
#include <numbers>
#include <cmath>
#include "ModelManager.h"
#include "TextureManager.h"
#include "BaseObjectCollider.h"
#include "CollisionConfig.h"
#include "TimeManager.h"
#include "Player.h"
#include "MyTools.h"

using namespace std;
using namespace numbers;
using namespace MyBase;

#pragma region 定数定義
const int Enemy::kMaxBulletCount = 10;								// 最大弾数
const float Enemy::kBulletDrawDistance = 150.0f;					// プレイヤーからの距離がこれ以上の弾は描画しない
const Vector3 Enemy::kBulletSpawnOffset = { 0.0f, 0.0f, -1.0f };	// 弾の発射位置オフセット

const Vector3 Enemy::kInitialPosition = { 10.0f, 0.0f, 500.0f };	// 敵の初期位置
const Vector3 Enemy::kInitialScale = { 1.0f, 1.0f, 1.0f };			// 敵の初期スケール
const Vector3 Enemy::kInitialRotation = { 0.0f, pi_v<float>, 0.0f };// 敵の初期回転
const Vector4 Enemy::kInitialColor = { 1.0f, 1.0f, 1.0f, 1.0f };	// 敵の初期色

const float Enemy::kColliderRadius = 1.50f;						// コライダーの半径

const int Enemy::kInitialHP = 10;				// 初期体力
const int Enemy::kInitialAttackPower = 10;		// 初期攻撃力

const Vector3 Enemy::kEmitterSize = { 1.0f, 1.0f, 1.0f };		// エミッターサイズ
const ParticleSystem::ParticleGroupData Enemy::kHitEffectParams = {
	.size = { 1.f, 10.0f },
	.energy = { 1.0f, 1.0f },
	.count = { 10, 15 },
	.speed = { 0.0f, 0.0f },
	.direction = { 0.0f, 0.0f, 0.0f },
	.color = { 0.88f, 0.28f, 0.0f, 1.0f },
	.frequency = 1.5f,
	.isBillboard = true,
	.isEmitUpdate = true
};		// ヒットエフェクトパーティクルパラメータ
const ParticleSystem::ParticleGroupData Enemy::kHitEffectRingParams = {
	.size = { 1.75f, 1.75f },
	.energy = { 1.0f, 1.0f },
	.count = { 1, 5 },
	.speed = { 0.0f, 0.0f },
	.direction = { 0.0f, 0.0f, 0.0f },
	.color = { 0.88f, 0.28f, 0.0f, 1.0f },
	.frequency = 1.5f,
	.isBillboard = true,
	.isEmitUpdate = true
};	// ヒットエフェクトパーティクルパラメータ(リング)

#ifdef _DEBUG
const float Enemy::kImGuiDragSpeed = 0.01f;				// ImGuiドラッグ速度
const ScopeF Enemy::kTranslateScope = { -100.0f, 100.0f };	// 平行移動範囲
const ScopeF Enemy::kRotateScope = { -pi_v<float>, pi_v<float> };		// 回転範囲
const ScopeF Enemy::kScaleScope = { 0.1f, 10.0f };		// スケール範囲
#endif // _DEBUG
#pragma endregion

Enemy::~Enemy() {
}

// 初期化
void Enemy::Initialize() {
	// ベースオブジェクトの初期化
	BaseObject::Initialize("enemy", "enemy.obj");
	
	// 3Dオブジェクトの初期化
	object_->SetTranslate(kInitialPosition);	// 初期位置
	object_->SetScale(kInitialScale);			// 初期スケール
	object_->SetRotate(kInitialRotation);		// 初期回転
	object_->SetColor(kInitialColor);			// 初期色

	// 敵のコライダーの初期化
	auto col = make_unique<BaseObjectCollider>(this);
	col->SetRadius(kColliderRadius); // 球体コライダー
	col->SetTypeId(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)); // コリジョン属性
	SetCollider(std::move(col)); // コライダーをセット
	
	// パーティクルエミッターの初期化
	particleEmitter_ = std::make_unique<ParticleEmitter>();
	particleEmitter_->Initialize("hitEffectEnemy", "circle.png", ParticleType::kEllipse);
	particleEmitter_->SetPosition(object_->GetTranslate());
	particleEmitter_->SetSize(kEmitterSize); // 初期サイズ
	ParticleSystem::ParticleGroupData hitEffect = kHitEffectParams;
	particleEmitter_->SetParticleGroupData("hitEffectEnemy", hitEffect);
	particleEmitter_->CreateParticleGroup("hitEffectRingEnemy", "gradationLine.png", ParticleType::kRing);
	particleEmitter_->SetPosition(object_->GetTranslate());
	particleEmitter_->SetSize(kEmitterSize); // 初期サイズ
	ParticleSystem::ParticleGroupData hitEffectRing = kHitEffectRingParams;
	particleEmitter_->SetParticleGroupData("hitEffectRingEnemy", hitEffectRing);

	// 弾の初期化
	bullets_.clear();

	// 敵のステータスの初期化
	hp_ = kInitialHP; // 初期HP
	isDead_ = false; // 初期状態は生存
	attackPower_ = kInitialAttackPower;
	attackCoolTime_ = kAttackCoolTime;
}

// 更新
void Enemy::Update() {
	// 敵の更新処理
	if (isDead_) {
		return; // 死んでいる場合は更新しない
	}

	// 攻撃
	Attack();

	// パーティクルエミッターの更新
	particleEmitter_->SetPosition(object_->GetTranslate());

	// 弾更新
	for (auto it = bullets_.begin(); it != bullets_.end(); ) {
		const bool isDead = (*it)->IsDead();

		if (isDead || (*it)->GetWorldPosition().z < player_->GetWorldPosition().z - kBulletDrawDistance) {
			it = bullets_.erase(it); // listから完全に削除
		}
		else {
			it->get()->Update(); // 弾の更新
			++it;
		}
	}

	// 攻撃のクールタイムを減らす
	if (attackCoolTime_ > 0.0f) {
		attackCoolTime_ -= TimeManager::GetInstance()->GetDeltaTime();
	}

	// ダメージリアクションの更新
	if(damageReactionTimer_ > 0.0f){
		// ダメージリアクションタイマーの更新
		damageReactionTimer_ -= TimeManager::GetInstance()->GetDeltaTime();
		DamageReactionUpdate();
	}

	// 死亡リアクションの更新
	if(deadReactionTimer_ > 0.0f){
		// 死亡リアクションタイマーの更新
		deadReactionTimer_ -= TimeManager::GetInstance()->GetDeltaTime();
		DeadReactionUpdate();
	}

	if (state_) {
		state_->Update();
	}

	// モデルの更新
	object_->Update();
}

// 描画
void Enemy::Draw() {
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
void Enemy::ChangeState(std::unique_ptr<EnemyBaseState> state) {
	state_ = std::move(state);
	state_->Initialize();
}

// ダメージ処理
void Enemy::Damage(int damage) {
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
void Enemy::DamageReactionStart() {
	// ダメージリアクションタイマーをリセット
	damageReactionTimer_ = kDamageReactionDuration;

	// パーティクルエミッターの更新
	particleEmitter_->Update();
	// パーティクルを発生させる
	particleEmitter_->Emit();

	// 赤を強調
	object_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
}

// ダメージリアクション
void Enemy::DamageReactionUpdate() {
	// ダメージリアクション終了確認
	if (damageReactionTimer_ <= 0.0f) {
		// タイマーリセットと色リセット
		damageReactionTimer_ = 0.0f;
		object_->SetColor(kInitialColor);
	}

	return;
}

// 死亡リアクション
void Enemy::DeadReactionStart() {
	// 死亡リアクションタイマーをリセット
	deadReactionTimer_ = kDeadReactionDuration;

	// パーティクルエミッターの更新
	particleEmitter_->Update();
	// パーティクルを発生させる
	particleEmitter_->Emit();

	// 赤を強調
	object_->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
}

void Enemy::DeadReactionUpdate() {
	// 死亡リアクション終了確認
	if (deadReactionTimer_ <= 0.0f) {
		// タイマーリセットと状態更新
		deadReactionTimer_ = 0.0f;
		object_->SetColor(kInitialColor);
		isDead_ = true;
	}
	return;
}

#ifdef _DEBUG
// デバッグ描画
void Enemy::DebugDraw() {
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Enemy")) {
		// ステータス表示
		ImGui::Text("HP: %d\n", hp_);
		ImGui::Text("Attack Power: %d\n", attackPower_);

		BaseObject::DebugDraw();

		ImGui::Text("\n");

		// ダメージ
		if (ImGui::Button("Damage")) {
			Damage(player_->GetAttackPower()); // ダメージを受ける
			if (hp_ <= 0) {
				DeadReactionStart(); // 死亡リアクション開始
			}
		}

		// パーティクルエミッターのデバッグUI
		if (particleEmitter_) {
			particleEmitter_->ImGui("Enemy");
		}
	}
	ImGui::PopID();
}
#endif // _DEBUG

void Enemy::Attack() {
	// 攻撃不可なら早期リターン
	if (!CanAttack()) return;
	// 弾の生成
	SpawnBullet();
}

bool Enemy::CanAttack() {
	if (bullets_.size() < kMaxBulletCount && attackCoolTime_ <= 0.0f) {
		return true;
	}
	return false;
}

void Enemy::SpawnBullet() {
	auto bullet = std::make_unique<EnemyBullet>();
	Vector3 direction = Matrix::TransformNormal({ 0.0f, 0.0f, 1.0f }, object_->GetWorldTransform()->GetWorldMatrix());
	bullet->Initialize(MyTools::Add(object_->GetTranslate(), direction), MyTools::Normalize(direction));
	bullet->SetAttackPower(attackPower_);
	bullets_.emplace_back(std::move(bullet));
	// 攻撃のクールタイムを設定
	attackCoolTime_ = kAttackCoolTime;
}

// 当たり判定
void Enemy::OnCollision([[maybe_unused]] Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeId();

	// プレイヤー又はプレイヤー弾が当たった時の処理
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer) ||			// プレイヤーの属性
		typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)) {	// プレイヤー弾の属性
		// プレイヤーの攻撃力分ダメージを受ける
		Damage(player_->GetAttackPower());
		if (hp_ <= 0) {
			DeadReactionStart(); // 死亡リアクション開始
		}
	}
}