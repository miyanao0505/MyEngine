#pragma once
#include "BaseObject.h"
#include <list>
#include "TextureManager.h"
#include "PlayerBullet.h"

// プレイヤー
class Player : public BaseObject
{
public:	// メンバ関数
	Player();
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	void Initialize(Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

#ifdef _DEBUG
	/// <summary>
	/// デバック描画
	/// </summary>
	void DebugDraw();
#endif // _DEBUG


	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attaack();

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

public:	// getter
	// 
	Vector3 GetWorldPosition() override { return BaseObject::GetWorldPosition(); };

public:	// setter


private:	// メンバ変数	
	// プレイヤーの弾リスト
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	
	// ステータス
	int hp_;
	bool isDead_ = false;
	
	// プレイヤーの移動速度
	const float kmoveSpeed_ = 0.2f;

	// プレイヤーの攻撃クールタイム
	const int kAttackCoolTime_ = 30;	// 30フレーム
	int32_t attackCoolTime_ = 0;		// 現在のクールタイム

	// 弾の上限
	const int kMaxBulletCount_ = 10; // 最大弾数
	// 弾の発射位置
	Vector3 bulletSpawnPosition_ = { 0.0f, 0.0f, 1.0f }; // プレイヤーの前方に発射
	// 弾の描画距離
	const float kBulletDrawDistance_ = 100.0f;

};

