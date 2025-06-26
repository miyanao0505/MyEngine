#include "Enemy.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include <imgui.h>

Enemy::Enemy()
{
	// 敵の初期化
	hp_ = 100; // 初期HP
	isDead_ = false; // 初期状態は生存
	SetCollisionAttribute(0x00000001); // コリジョン属性
	SetCollisionMask(0x00000001); // コリジョンマスク

}

Enemy::~Enemy()
{

}

// 初期化
void Enemy::Initialize()
{
	// 敵の初期化
	SetRadius(1.0f); // 半径1.0fの球体コライダー
	SetSize({ 1.0f, 1.0f });
	SetCollisionAttribute(0x00000001); // コリジョン属性
	SetCollisionMask(0x00000001); // コリジョンマスク

	// モデルの初期化
	ModelManager::GetInstance()->LoadModel("resources/model/debug/sphere", "sphere.obj");
	TextureManager::GetInstance()->LoadTexture("resources/texture/uvChecker.png");

	object_ = std::make_unique<Object3d>();
	object_->Initislize("sphere.obj");
	object_->SetTexture("resources/texture/uvChecker.png");
	object_->SetTranslate({ 0.0f, 0.0f, 15.0f }); // 初期位置
	object_->SetScale({ 1.0f, 1.0f, 1.0f }); // 初期スケール
}

// 更新
void Enemy::Update()
{
	// 敵の更新処理
	if (isDead_) {
		return; // 死んでいる場合は更新しない
	}

	// ここに敵の動きやAIのロジックを追加する
	// モデルの更
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
}

#ifdef _DEBUG
// デバッグ描画
void Enemy::DebugDraw()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Enemy"))
	{
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

// 当たり判定
void Enemy::OnCollision()
{

}
