#include "Enemy.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "BaseObjectCollider.h"
#include "CollisionConfig.h"
#include <imgui.h>

using namespace std;

Enemy::Enemy()
{
	Initialize();
}

Enemy::~Enemy()
{

}

// 初期化
void Enemy::Initialize()
{
	// モデルの初期化
	ModelManager::GetInstance()->LoadModel("resources/model/debug/sphere", "sphere.obj");
	TextureManager::GetInstance()->LoadTexture("resources/texture/uvChecker.png");

	// ベースオブジェクトの初期化
	BaseObject::Initialize("sphere.obj");
	
	// 3Dオブジェクトの初期化
	object_->Initislize("sphere.obj");
	object_->SetTexture("resources/texture/uvChecker.png");
	object_->SetTranslate({ 0.0f, 0.0f, 15.0f }); // 初期位置
	object_->SetScale({ 0.50f, 0.50f, 0.50f }); // 初期スケール

	// 敵のコライダーの初期化
	auto col = make_unique<BaseObjectCollider>(this);
	col->SetRadius(1.0f); // 半径1.0fの球体コライダー
	col->SetSize({ 1.0f, 1.0f });
	col->SetTypeId(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)); // コリジョン属性
	SetCollider(std::move(col)); // コライダーをセット
	
	// パーティクルエミッターの初期化
	particleEmitter_ = std::make_unique<ParticleEmitter>();
	particleEmitter_->Initialize("hitEffectEnemy", "resources/texture/circle.png", ParticleEmitter::Ellipse);
	particleEmitter_->SetPosition(object_->GetTranslate());
	particleEmitter_->SetSize({ 1.0f, 1.0f, 1.0f }); // 初期サイズ
	ParticleSystem::ParticleGroupData hitEffect = {
		.size = { 1.f, 5.0f },
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
	particleEmitter_->CreateParticleGroup("hitEffectRingEnemy", "resources/texture/gradationLine.png", ParticleEmitter::Ring);
	particleEmitter_->SetPosition(object_->GetTranslate());
	particleEmitter_->SetSize({ 1.0f, 1.0f, 1.0f }); // 初期サイズ
	ParticleSystem::ParticleGroupData hitEffectRing = {
		.size = { 1.5f, 1.5f },
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

	// 敵のステータスの初期化
	hp_ = 100; // 初期HP
	isDead_ = false; // 初期状態は生存
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

		// パーティクルエミッターのデバッグUI
		if (particleEmitter_) {
			particleEmitter_->Imgui("Enemy");
		}
	}
	ImGui::PopID();
}
#endif // _DEBUG

// 当たり判定
void Enemy::OnCollision([[maybe_unused]] Collider* other)
{
	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeId();

	// プレイヤー弾が当たった時の処理
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayerBullet)) { // プレイヤー弾の属性
		particleEmitter_->Emit(); // パーティクルを発生させる
	}
}
