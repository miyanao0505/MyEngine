#pragma once
#include "BaseObject.h"
#include "BulletOwner.h"
#include "MyBase.h"

/// <summary>
/// 弾の基底クラス
/// </summary>
class BaseBullet : public BaseObject {
public:	// クラス関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseBullet() = default;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

public:	// getter
	/// <summary>
	/// 弾が死亡しているかを取得
	/// </summary>
	/// <returns>弾が死亡しているか</returns>
	bool IsDead() const { return isDead_; }

	/// <summary>
	/// 弾の所有者を取得
	/// </summary>
	/// <returns>弾の所有者</returns>
	BulletOwner GetOwner() const { return owner_; }

protected:	// メンバ変数
	std::unique_ptr<BaseObject> object_;	// 弾のモデル

	MyBase::Vector3 direction_;	// 弾の移動方向

	float speed_;			// 弾の速度
	float damage_;			// 弾のダメージ量

	bool isDead_ = false;	// 弾が死亡しているか
	BulletOwner owner_;		// 弾の所有者
};