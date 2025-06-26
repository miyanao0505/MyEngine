#include "Player.h"
#include "Input.h"
#include "ModelManager.h"
#include "MyTools.h"
#include "imgui.h"

Player::Player()
{
	// プレイヤーの初期化
	hp_ = 100;
	isDead_ = false;
	SetCollisionAttribute(0x00000001); // コリジョン属性
	SetCollisionMask(0x00000001); // コリジョンマスク
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
	SetCollisionAttribute(0x00000001);
	SetCollisionMask(0x00000001);
	// モデルの初期化
	ModelManager::GetInstance()->LoadModel("resources/model/debug/sphere", "sphere.obj");
	
	object_ = std::make_unique<Object3d>();
	object_->Initislize("sphere.obj");
	//object_->SetTexture("monsterBall.png");
	object_->SetTranslate(position);
	object_->SetScale({ 1.0f, 1.0f, 1.0f });
}

/// 更新
void Player::Update()
{
	// 移動処理
	Move();

	object_->Update();
}

/// 描画
void Player::Draw()
{
	// プレイヤー
	object_->Draw();
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

}

/// 衝突を検出したら呼び出されるコールバック関数
void Player::OnCollision()
{

}
