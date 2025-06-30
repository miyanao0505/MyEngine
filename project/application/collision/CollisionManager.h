#pragma once
#include "Collider.h"
#include <list>

class CollisionManager
{
public:	// メンバ関数
	CollisionManager();
	~CollisionManager();

	// 衝突判定と応答
	void CheckAllCollisions();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	// コライダーリストをクリア
	void Clear();

public:	// setter
	// コライダーリストに登録する
	void AddCollider(Collider* collider) { colliders_.push_back(collider); }

private:	// メンバ変数
	// コライダーリスト
	std::list<Collider*> colliders_;

};

