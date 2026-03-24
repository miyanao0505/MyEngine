#include "Player.h"
#include "Input.h"
#include "ModelManager.h"
#include "BaseObjectCollider.h"
#include "CollisionConfig.h"
#include "MyTools.h"
#include <imgui.h>
#include <numbers>

using namespace std;
using namespace numbers;

#pragma region 定数
const MyBase::Vector3 Player::kInitialScale = { 1.0f, 1.0f, 1.0f };
const float Player::kColliderRadius = 1.0f;

const int Player::kInitialHP = 30;
const int Player::kInitialAttackPower = 10;

#ifdef _DEBUG
const float Player::kDebugMoveSpeed = 0.01f;
const MyBase::ScopeF Player::kTranslateScope = { -100.0f, 100.0f };
const MyBase::ScopeF Player::kRotateScope = { -pi_v<float>, pi_v<float> };
const MyBase::ScopeF Player::kScaleScope = { 0.01f, 10.0f };
#endif // _DEBUG
#pragma endregion

/// デストラクタ
Player::~Player()
{

}

/// 初期化
void Player::Initialize(const MyBase::Vector3& position)
{
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

	// 弾の初期化
	bullets_.clear();

	// ステータスの初期化
	hp_ = kInitialHP;
	attackPower_ = kInitialAttackPower;
	attackCoolTime_ = kAttackCoolTime;
	isDead_ = false;
}

/// 更新
void Player::Update(float deltaTime)
{
	// 移動処理
	ReadMoveInput();

	// オブジェクトの更新
	object_->Update();

	// 攻撃
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
}

/// 描画
void Player::Draw()
{
	// プレイヤー
	object_->Draw();

	// 弾の描画
	for (auto it = bullets_.begin(); it != bullets_.end(); ) {
		it->get()->Draw();
		++it;
	}
}

/// ダメージ処理
void Player::Damage(int damage)
{
	hp_ -= damage;
	if (hp_ <= 0) {
		isDead_ = true;
		hp_ = 0;
	}
}

#ifdef _DEBUG
/// デバック描画
void Player::DebugDraw()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Player"))
	{
		// ステータス表示
		ImGui::Text("HP: %d", hp_);
		ImGui::Text("Attack Power: %d", attackPower_);
		ImGui::Text("\n");

		MyBase::Transform transform = {object_->GetScale(), object_->GetRotate(), object_->GetTranslate()};

		// 移動
		ImGui::DragFloat3("Translate", &transform.translate.x, kDebugMoveSpeed, kTranslateScope.min, kTranslateScope.max);
		// 回転
		ImGui::DragFloat3("Rotate", &transform.rotate.x, kDebugMoveSpeed, kRotateScope.min, kRotateScope.max);
		// 拡縮
		ImGui::DragFloat3("Scale", &transform.scale.x, kDebugMoveSpeed, kScaleScope.min, kScaleScope.max);
		object_->SetTransform(transform);

		ImGui::Text("\n");
	}
	ImGui::PopID();
}
#endif // _DEBUG


/// 移動処理
void Player::ReadMoveInput()
{
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
void Player::Attack()
{
	// 攻撃不可なら早期リターン
	if (!CanAttack()) return;
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		// 弾の生成
		SpawnBullet();
	}
}

/// 攻撃可能かどうか
bool Player::CanAttack()
{
	if (bullets_.size() < kMaxBulletCount && attackCoolTime_ <= 0) {
		return true;
	}
	return false;
}

/// 弾生成
void Player::SpawnBullet()
{
	auto bullet = std::make_unique<PlayerBullet>();
	MyBase::Vector3 direction = Matrix::TransformNormal(kBulletOffset, object_->GetWorldTransform()->GetWorldMatrix());
	bullet->Initialize(MyTools::Add(object_->GetTranslate(), direction), MyTools::Normalize(direction));
	bullets_.emplace_back(std::move(bullet));
	// 攻撃のクールタイムを設定
	attackCoolTime_ = kAttackCoolTime;
}

/// 衝突を検出したら呼び出されるコールバック関数
void Player::OnCollision([[maybe_unused]] Collider* other)
{
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
void Player::SetWorldPosition(const MyBase::Vector3& pos)
{
	externalPosition_ = pos;
	object_->SetTranslate(pos);
	object_->Update();
}