#include "Player.h"
#include "Input.h"
#include "ModelManager.h"
#include"CollisionConfig.h"
#include "MyTools.h"
#include "imgui.h"

Player::Player()
{
	// プレイヤーの初期化
	Initialize({ 0.0f, 0.0f, 0.0f });
}

/// デストラクタ
Player::~Player()
{

}

/// 初期化
void Player::Initialize(MyBase::Vector3 position)
{
	// プレイヤーのコライダーの初期化
	SetRadius(1.0f);
	SetSize({ 1.0f, 1.0f });
	SetTypeId(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));
	// モデルの初期化
	ModelManager::GetInstance()->LoadModel("resources/model/player", "player.obj");
	
	// プレイヤーのオブジェクトの初期化
	object_ = std::make_unique<Object3d>();
	object_->Initislize("player.obj");
	//object_->SetTexture("monsterBall.png");
	object_->SetTranslate(position);
	object_->SetScale({ 0.5f, 0.5f, 0.5f });

	// 弾の初期化
	bullets_.clear();

	// ステータスの初期化
	hp_ = 100;
	attackCoolTime_ = 0;
	isDead_ = false;
}

/// 更新
void Player::Update()
{
	// 移動処理
	Move();

	// オブジェクトの更新
	object_->Update();

	// 攻撃
	Attaack();

	// 弾更新
	for (auto it = bullets_.begin(); it != bullets_.end(); ) {
		const bool isDead = (*it)->IsDead();
		const float distance = MyTools::Length(MyTools::Subtract((*it)->GetWorldPosition(), GetWorldPosition()));

		if (isDead || distance >= kBulletDrawDistance_) {
			it = bullets_.erase(it); // listから完全に削除
		}
		else {
			it->get()->Update(); // 弾の更新
			++it;
		}
	}

	// 攻撃のクールタイムを減らす
	if (attackCoolTime_ > 0) {
		attackCoolTime_--;
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

#ifdef _DEBUG
/// デバック描画
void Player::DebugDraw()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Player"))
	{
		MyBase::Transform transform = {object_->GetScale(), object_->GetRotate(), object_->GetTranslate()};

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


/// 移動処理
void Player::Move()
{
	// プレイヤーの移動
	MyBase::Vector3 velocity = { 0.0f, 0.0f, 0.0f };
	if (Input::GetInstance()->PushKey(DIK_W)) {
		velocity.y += kmoveSpeed_;
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		velocity.y -= kmoveSpeed_;
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		velocity.x -= kmoveSpeed_;
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		velocity.x += kmoveSpeed_;
	}
	
	// 座標更新
	Vector3 newPos = MyTools::Add(object_->GetTranslate(), velocity);
	object_->SetTranslate(newPos);
}

/// 攻撃
void Player::Attaack()
{
	if (Input::GetInstance()->PushKey(DIK_SPACE) && bullets_.size() < kMaxBulletCount_ && attackCoolTime_ <= 0)
	{
		// 弾の生成
		auto bullet = std::make_unique<PlayerBullet>();
		bullet->Initialize(object_->GetTranslate());
		bullets_.emplace_back(std::move(bullet));
		// 攻撃のクールタイムを設定
		attackCoolTime_ = kAttackCoolTime_;
	}
}

/// 衝突を検出したら呼び出されるコールバック関数
void Player::OnCollision([[maybe_unused]] Collider* other)
{

}
