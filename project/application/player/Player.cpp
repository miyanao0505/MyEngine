#include "Player.h"
#include "Input.h"
#include "ModelManager.h"
#include "BaseObjectCollider.h"
#include "CollisionConfig.h"
#include "BulletManager.h"
#include "StraightBullet.h"
#include "BulletType.h"
#include "BulletOwner.h"
#include "TimeManager.h"
#include "Reticle.h"
#include "MyTools.h"
#include <imgui.h>
#include <numbers>

using namespace std;
using namespace numbers;
using namespace MyBase;

#pragma region 定数
const Vector3 Player::kInitialScale = { 1.0f, 1.0f, 1.0f };
const float Player::kColliderRadius = 1.0f;

const int Player::kInitialHP = 50;
const int Player::kInitialAttackPower = 10;

#ifdef _DEBUG
const float Player::kDebugMoveSpeed = 0.01f;
const ScopeF Player::kTranslateScope = { -100.0f, 100.0f };
const ScopeF Player::kRotateScope = { -pi_v<float>, pi_v<float> };
const ScopeF Player::kScaleScope = { 0.01f, 10.0f };
#endif // _DEBUG
#pragma endregion

/// デストラクタ
Player::~Player() {

}

/// 初期化
void Player::Initialize(const Vector3& position) {
	// ベースオブジェクトの初期化
	BaseObject::Initialize("player", "player.obj");
	SetName("Player");

	// プレイヤーのオブジェクトの設定
	object_->SetTranslate(position);
	object_->SetScale(kInitialScale);

	// プレイヤーのコライダーの初期化
	auto col = make_unique<BaseObjectCollider>(this);
	col->SetRadius(kColliderRadius); // 球体コライダー
	col->SetTypeId(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));
	SetCollider(std::move(col)); // コライダーをセット

	// ステータスの初期化
	hp_ = kInitialHP;
	attackPower_ = kInitialAttackPower;
	attackCoolTime_ = kAttackCoolTime;
	isDead_ = false;
}

/// 更新
void Player::Update() {
	// 移動処理
	ReadMoveInput();

	// オブジェクトの更新
	object_->Update();

	// 攻撃
	Attack();

	// 攻撃のクールタイムを減らす
	if (attackCoolTime_ > 0) {
		attackCoolTime_ -= TimeManager::GetInstance()->GetDeltaTime();
	}

	// ダメージリアクションの更新
	if (damageReactionTimer_ > 0.0f) {
		// ダメージリアクションタイマーの更新
		damageReactionTimer_ -= TimeManager::GetInstance()->GetDeltaTime();
		DamageReactionUpdate();
	}
}

/// 描画
void Player::Draw() {
	// プレイヤー
	object_->Draw();
}

/// ダメージ処理
void Player::Damage(int damage) {
	if (damageReactionTimer_ > 0.0f) {
		return; // ダメージリアクション中はダメージを受け付けない
	}

	hp_ -= damage;

	if (hp_ > 0.0f) {
		// ダメージリアクション開始
		DamageReactionStart();
	}
}

/// ダメージリアクションの開始
void Player::DamageReactionStart() {
	// ダメージリアクションタイマーをリセット
	damageReactionTimer_ = kDamageReactionDuration;

	// 赤を強調
	object_->SetColor({1.0f, 0.5f, 0.5f, 1.0f});
}

/// ダメージリアクションの更新
void Player::DamageReactionUpdate() {
	// ダメージリアクション終了確認
	if (damageReactionTimer_ <= 0.0f) {
		// タイマーリセットと色リセット
		damageReactionTimer_ = 0.0f;
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	return;
}

/// 死亡リアクション
void Player::DeadReaction() {

}

#ifdef _DEBUG
/// デバック描画
void Player::DebugDraw() {
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Player"))
	{
		// ステータス表示
		ImGui::Text("HP: %d", hp_);
		ImGui::Text("Attack Power: %d", attackPower_);
		ImGui::Text("\n");

		BaseObject::DebugDraw();

		ImGui::Text("\n");
	}
	ImGui::PopID();
}
#endif // _DEBUG


/// 移動処理
void Player::ReadMoveInput() {
	moveInput_ = { 0.0f, 0.0f };

	if (Input::GetInstance()->PushKey(DIK_W)) moveInput_.y += kMoveSpeed;
	if (Input::GetInstance()->PushKey(DIK_S)) moveInput_.y -= kMoveSpeed;
	if (Input::GetInstance()->PushKey(DIK_D)) moveInput_.x += kMoveSpeed;
	if (Input::GetInstance()->PushKey(DIK_A)) moveInput_.x -= kMoveSpeed;

	// 正規化
	if (MyTools::Length(moveInput_) > 1.0f) {
		moveInput_ = MyTools::Normalize(moveInput_);
	}
}

/// 攻撃
void Player::Attack() {
	// 攻撃不可なら早期リターン
	if (!CanAttack()) return;
	if (Input::GetInstance()->PushKey(DIK_SPACE) ||
		Input::GetInstance()->PushMouse(MouseButton::Left)) {
		// 弾の生成
		SpawnBullet();
	}
}

/// 攻撃可能かどうか
bool Player::CanAttack() {
	if (BulletManager::GetInstance()->GetPlayerBulletCount() < kMaxBulletCount && attackCoolTime_ <= 0) {
		return true;
	}
	return false;
}

/// 弾生成
void Player::SpawnBullet() {
	auto bullet = std::make_unique<StraightBullet>(BulletOwner::Player);
	Vector3 position = MyTools::Add(object_->GetTranslate(), MyTools::Normalize(Matrix::TransformNormal(kBulletOffset, object_->GetWorldTransform()->GetWorldMatrix())));
	Vector3 direction = MyTools::Normalize(MyTools::Subtract(reticle_->GetWorldPosition(), position));
	bullet->Initialize(position, direction);
	bullet->SetSpeed(kBulletSpeed);
	bullet->SetDamage(attackPower_);
	BulletManager::GetInstance()->AddBullet(std::move(bullet));

	// 攻撃のクールタイムを設定
	attackCoolTime_ = kAttackCoolTime;
}

/// 衝突を検出したら呼び出されるコールバック関数
void Player::OnCollision([[maybe_unused]] Collider* other) {
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeId();

	// 敵 or 敵弾が当たった時の処理
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy) ||			// 敵の属性
		typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet)) {	// 敵弾の属性
		// 敵の攻撃力分ダメージを受ける
		Damage(10);
		if (hp_ <= 0) {
			isDead_ = true;
		}
	}
}

/// ワールド座標を設定
void Player::SetWorldPosition(const Vector3& pos) {
	externalPosition_ = pos;
	object_->SetTranslate(pos);
	object_->Update();
}