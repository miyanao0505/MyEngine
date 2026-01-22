#pragma once
#include "BaseObject.h"

/// <summary>
/// 敵が発射する弾を表すクラス。 
/// BaseObjectを継承し、弾の移動・寿命管理・衝突判定・描画を担当します。
/// </summary>
class EnemyBullet : public BaseObject
{
private:
#pragma region 定数
	static constexpr float kMoveSpeed = 50.0f;	// 弾の移動速度
	static constexpr float kLifeTime = 20.0f;	// 寿命
	
	static const MyBase::Vector3 kInitialScale;		// 弾の初期スケール
	static const float kColliderRadius;	// コライダーの半径

#ifdef _DEBUG

#endif // _DEBUG
#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyBullet() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	void Initialize(const MyBase::Vector3& position, const MyBase::Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaTime">前フレームからの経過時間[秒]</param>
	void Update(float deltaTime) override;

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
	/// <param name="deltaTime">前フレームからの経過時間[秒]</param>
	void Move(float deltaTime);

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

public:	// getter
	/// <summary>
	/// オブジェクトのワールド座標を取得
	/// </summary>
	/// <returns>オブジェクトのワールド座標(Vector3)</returns>
	MyBase::Vector3 GetWorldPosition() const override { return BaseObject::GetWorldPosition(); };

	/// <summary>
	/// 攻撃力を取得
	/// </summary>
	/// <returns>攻撃力</returns>
	int GetAttackPower() const { return attackPower_; }

	/// <summary>
	/// 弾が消滅したかどうかを取得
	/// </summary>
	/// <returns>true:消滅している、false:消滅していない</returns>
	bool IsDead() const { return isDead_; }	// 弾が消滅したかどうか

public:	// setter

	void SetAttackPower(int attackPower) { attackPower_ = attackPower; }

private:	// メンバ変数
	// ステータス
	bool isDead_ = false;
	int attackPower_;

	// 弾の速度
	MyBase::Vector3 velocity_;

	// デスタイマー
	float deathTimer_ = kLifeTime;
};