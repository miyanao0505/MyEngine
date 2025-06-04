#include "Enemy.h"
#include "ModelManager.h"
#include "TextureManager.h"

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

void Enemy::Update()
{
	// 敵の更新処理
	if (isDead_) {
		return; // 死んでいる場合は更新しない
	}
	// ここに敵の動きやAIのロジックを追加する
	object_->Update(); // モデルの更新

}

void Enemy::Draw()
{
	// 敵の描画処理
	if (isDead_) {
		return; // 死んでいる場合は描画しない
	}
	object_->Draw(); // モデルの描画

}

void Enemy::DebugDraw()
{

}

void Enemy::OnCollision()
{

}
