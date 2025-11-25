#pragma once
#include "Collider.h"
#include <list>

/// <summary>
/// CollisionManagerクラス
/// ゲーム全体の衝突判定を統括管理するシングルトンマネージャー。
/// </summary>
class CollisionManager
{
public:	// メンバ関数
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static CollisionManager* GetInstance();
	
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// Colliderの登録
	/// </summary>
	/// <param name="collider">コライダー</param>
	void Register(Collider* collider) { colliders_.push_back(collider); }

	/// <summary>
	/// Colliderの解除
	/// </summary>
	/// <param name="collider">コライダー</param>
	void Unregister(Collider* collider) { colliders_.remove(collider); }

	/// <summary>
	/// コライダーリストをクリア
	/// </summary>
	void Clear();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
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

