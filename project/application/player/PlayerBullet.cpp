#include "PlayerBullet.h"
#include "ModelManager.h"
#include "MyTools.h"
#include "BaseObjectCollider.h"
#include "CollisionConfig.h"
#include "TimeManager.h"
#include <imgui.h>
#include <numbers>

using namespace std;
using namespace numbers;

#pragma region 定数
const MyBase::Vector3 PlayerBullet::kInitialScale{ 0.5f, 0.5f, 0.5f };
const float PlayerBullet::kColliderRadius = 0.50f;
#ifdef _DEBUG
const float PlayerBullet::kDebugMoveSpeed = 0.01f;
const MyBase::ScopeF PlayerBullet::kTranslateScope = { -100.0f, 100.0f };
const MyBase::ScopeF PlayerBullet::kRotateScope = { -pi_v<float>, pi_v<float> };
const MyBase::ScopeF PlayerBullet::kScaleScope = { 0.0f, 10.0f };
#endif // _DEBUG
#pragma endregion

// 初期化
void PlayerBullet::Initialize(const MyBase::Vector3& position, const MyBase::Vector3& velocity)
{
	// ベースオブジェクトの初期化
	BaseObject::Initialize("debug/sphere", "sphere.obj");

	object_->SetTexture("playerBullet.png");
	object_->SetTranslate(position);
	object_->SetScale(kInitialScale);

	velocity_ = velocity;

	// プレイヤー弾のコライダーの初期化
	auto col = make_unique<BaseObjectCollider>(this);
	col->SetRadius(kColliderRadius); // 球体コライダー
	col->SetTypeId(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)); // プレイヤー弾
	SetCollider(std::move(col)); // コライダーをセット

	deathTimer_ = kLifeTime;
}

// 更新
void PlayerBullet::Update()
{
	if (isDead_) return; // 弾が消滅している場合は更新しない

	// 移動処理
	Move(TimeManager::GetInstance()->GetDeltaTime());

	// 寿命を減らす
	deathTimer_ -= TimeManager::GetInstance()->GetDeltaTime();
	// 寿命が尽きたら消滅フラグを立てる
	if (deathTimer_ <= 0.0f) {
		isDead_ = true;
	}

	// オブジェクトの更新
	object_->Update();
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

// 移動処理
void PlayerBullet::Move(float deltaTime)
{
	// 弾の移動
	MyBase::Vector3 move = MyTools::Multiply(deltaTime * kMoveSpeed, velocity_);
	object_->SetTranslate(MyTools::Add(object_->GetTranslate(), move));
}

// 当たり判定
void PlayerBullet::OnCollision([[maybe_unused]] Collider* other)
{
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeId();

	// プレイヤー弾が敵に当たったら削除
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy) ||			// 敵の属性
		typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet)) {	// 敵弾の属性
		isDead_ = true;
	}
}