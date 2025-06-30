#pragma once
#include "Collider.h"
#include "Object3d.h"

class PlayerBullet : public Collider
{
public:	// メンバ関数
	virtual ~PlayerBullet() = default;	// 仮想デストラクタ

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	void Initialize(MyBase::Vector3 position);

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
	/// 当たり判定
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

public:	// getter
	Vector3 GetWorldPosition() override { return object_->GetTranslate(); };

	bool IsDead() const { return isDead_; }	// 弾が消滅したかどうか

public:	// setter


private:	// メンバ変数
	// モデル
	std::unique_ptr<Object3d> object_ = nullptr;

	// ステータス
	bool isDead_ = false;

	// 弾の移動速度
	const float kmoveSpeed_ = 0.2f;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
};

