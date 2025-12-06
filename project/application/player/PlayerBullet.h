#pragma once
#include "BaseObject.h"

/// <summary>
/// プレイヤーが発射する弾を表すクラス。 
/// BaseObjectを継承し、弾の移動・寿命管理・衝突判定・描画を担当します。
/// </summary>
class PlayerBullet : public BaseObject
{
public:	// メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerBullet() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	void Initialize(const MyBase::Vector3& position, const MyBase::Vector3& velocity);

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
	void Move();

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
	/// 弾が消滅したかどうかを取得
	/// </summary>
	/// <returns>true:消滅している、false:消滅していない</returns>
	bool IsDead() const { return isDead_; }	// 弾が消滅したかどうか

public:	// setter


private:	// メンバ変数
	// ステータス
	bool isDead_ = false;

	// 弾の移動速度
	const float kMoveSpeed = 0.2f;

	// 弾の速度
	MyBase::Vector3 velocity_;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
};