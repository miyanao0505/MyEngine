#include "CollisionManager.h"
#include "MyTools.h"

using namespace std;

/// static member 定義
unique_ptr<CollisionManager> CollisionManager::sInstance_ = nullptr;

/// Singleton Instance を取得
CollisionManager* CollisionManager::GetInstance() {
	if (sInstance_ == nullptr) {
		sInstance_ = make_unique<CollisionManager>(CollisionManager::ConstructorKey{});
	}
	return sInstance_.get();
}

// 終了
void CollisionManager::Finalize() {
	// コライダーリストをクリア
	Clear();
	// シングルトンインスタンスを削除
	sInstance_.reset();
}

// 更新
void CollisionManager::Update() {
	// コライダーリストが空なら何もしない
	if (colliders_.empty()) return;

	// 衝突判定と応答を行う
	CheckAllCollisions();
}

// 衝突判定と応答
void CollisionManager::CheckAllCollisions() {
	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*> ::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			// イテレータBからコライダーBを取得する
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

// コライダー2つの衝突判定と応答
void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	// 衝突判定が無効なら早期リターン
	if (!colliderA->IsCollisionEnabled() || !colliderB->IsCollisionEnabled()) return;

	// コライダーAとBのワールド座標を取得
	MyBase::Vector3 posA = colliderA->GetWorldPosition();
	MyBase::Vector3 posB = colliderB->GetWorldPosition();

	// 距離を求める
	float length = MyTools::Length(MyTools::Subtract(posB, posA));

	// 球と球の交差判定
	if (length <= colliderA->GetRadius() + colliderB->GetRadius()) {
		// 衝突した場合の処理
		colliderA->OnCollision(colliderB);
		colliderB->OnCollision(colliderA);
	}
}

// コライダーリストをクリア
void CollisionManager::Clear() {
	colliders_.clear();
}
