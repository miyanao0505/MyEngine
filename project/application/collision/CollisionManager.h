#pragma once
#include "Collider.h"
#include <list>

class CollisionManager
{
public:	// メンバ関数
	// シングルトンインスタンスの取得
	static CollisionManager* GetInstance();
	// 終了
	void Finalize();

	// Colliderの登録
	void Register(Collider* collider) { colliders_.push_back(collider); }

	// Colliderの解除
	void Unregister(Collider* collider) { colliders_.remove(collider); }

	// コライダーリストをクリア
	void Clear();

	// 更新
	void Update();

	// 衝突判定と応答
	void CheckAllCollisions();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

private:	// シングルトン
	static CollisionManager* instance;

	CollisionManager() = default;
	~CollisionManager() = default;
	CollisionManager(CollisionManager&) = delete;
	CollisionManager& operator=(CollisionManager&) = delete;

private:	// メンバ変数
	// コライダーリスト
	std::list<Collider*> colliders_;

};

