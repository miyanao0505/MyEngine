#pragma once
#include "BaseBullet.h"

/// <summary>
/// 直線弾クラス
/// </summary>
class StraightBullet : public BaseBullet {
private:	// 定数
#pragma region 定数
	static constexpr float kMoveSpeed = 150.0f;		// 弾の移動速度
	static constexpr float kLifeTime = 5.0f;		// 弾の寿命
#pragma endregion

public:	// クラス関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">弾の所有者</param>
	StraightBullet(BulletOwner owner);
		
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">弾の初期位置</param>
	/// <param name="direction">弾の初期方向</param>
	void Initialize(const MyBase::Vector3& position, const MyBase::Vector3& direction);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突判定処理
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision([[maybe_unused]] Collider* other) override;

public:	// getter
	/// <summary>
	/// 弾の移動速度を取得
	/// </summary>
	/// <returns>弾の移動速度</returns>
	float GetSpeed() const { return speed_; }

	/// <summary>
	/// 弾のダメージ量を取得
	/// </summary>
	/// <returns>弾のダメージ量</returns>
	int GetDamage() const { return damage_; }

public:	// setter
	/// <summary>
	/// 弾の移動速度を設定
	/// </summary>
	/// <param name="speed">弾の移動速度</param>
	void SetSpeed(float speed) { speed_ = speed; }

	/// <summary>
	/// 弾のダメージ量を設定
	/// </summary>
	/// <param name="damage">弾のダメージ量</param>
	void SetDamage(int damage) { damage_ = damage; }

private:	// メンバ関数
	/// <summary>
	/// 弾の移動処理
	/// </summary>
	void Move();
};