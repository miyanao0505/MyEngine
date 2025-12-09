#pragma once
#include "BaseObject.h"
#include <list>
#include "TextureManager.h"
#include "PlayerBullet.h"

/// <summary>
/// プレイヤーキャラクターを表すクラス。
/// 自機の移動・回転・攻撃・衝突処理など、ゲーム中の主要な操作と挙動を管理します。
/// </summary>
class Player : public BaseObject
{
public:	// メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	void Initialize(const MyBase::Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

#ifdef _DEBUG
	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw() override;
#endif // _DEBUG

	/// <summary>
	/// 移動処理
	/// </summary>
	void HandleMovementInput();

	/// <summary>
	/// 回転処理
	/// </summary>
	void HandleRotationInput();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

public:	// getter
	/// <summary>
	/// 攻撃力を取得
	/// </summary>
	/// <returns>攻撃力を取得</returns>
	int GetAttackPower() const { return attackPower_; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	MyBase::Vector3 GetWorldPosition() const override { return BaseObject::GetWorldPosition(); };

public:	// setter
	/// <summary>
	/// 攻撃力を設定
	/// </summary>
	/// <param name="attackPower">攻撃力</param>
	void SetAttackPower(int attackPower) { attackPower_ = attackPower; }

private:	// メンバ変数	
	// プレイヤーの弾リスト
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	
	// ステータス
	int hp_;
	int attackPower_ = 1;
	bool isDead_ = false;
	
	// プレイヤーの移動速度
	const float kMoveSpeed = 0.2f;

	// プレイヤーの攻撃クールタイム
	const int kAttackCoolTime = 30;	// 30フレーム
	int32_t attackCoolTime_ = 0;		// 現在のクールタイム

	// 弾の上限
	const int kMaxBulletCount = 10; // 最大弾数
	// 弾の発射位置
	MyBase::Vector3 bulletSpawnPosition_ = { 0.0f, 0.0f, 1.0f }; // プレイヤーの前方に発射
	// 弾の描画距離
	const float kBulletDrawDistance = 100.0f;
};