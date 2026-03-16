#pragma once
#include <list>
#include <memory>
#include "Collider.h"

/// <summary>
/// CollisionManagerクラス
/// ゲーム全体の衝突判定を統括管理するマネージャークラス。
/// </summary>
class CollisionManager
{
public:	// メンバ関数
	/// <summary>
	/// Singleton Instance を取得
	/// </summary>
	/// <returns>CollisionManager</returns>
	static CollisionManager* GetInstance();
	
	/// ------ Passkey Idion ------
	/// コントラクタを渡すための鍵
	class ConstructorKey {
	private:
		ConstructorKey() = default;
		friend class CollisionManager;
	};

	/// PassKeyを受け取るコンストラクタ
	explicit CollisionManager(ConstructorKey) {}

	/// コピー禁止
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// Colliderの登録
	/// </summary>
	/// <param name="collider">Collider</param>
	void Register(Collider* collider) { colliders_.push_back(collider); }

	/// <summary>
	/// Colliderの解除
	/// </summary>
	/// <param name="collider">Collider/param>
	void Unregister(Collider* collider) { colliders_.remove(collider); }

	/// <summary>
	/// Colliderリストをクリア
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
	/// Collider2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">ColliderA</param>
	/// <param name="colliderB">ColliderB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

private:	// Singleton Instance
	static std::unique_ptr<CollisionManager>sInstance_;

private:	// メンバ変数
	// コライダーリスト
	std::list<Collider*> colliders_;
};