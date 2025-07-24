#pragma once
#include "MyBase.h"
#include "Quaternion.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider : public MyBase
{
public:	// メンバ関数
	Collider();
	virtual ~Collider();

	// 衝突時のコールバック関数
	virtual void OnCollision([[maybe_unused]] Collider* other) {};

public:	// gettere
	// 半径を取得
	float GetRadius() const { return radius_; }
	// AABBを取得
	const AABB& GetAABB() const { return aabb_; }
	// OBBを取得
	const OBB& GetOBB() const { return obb_; }
	// ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;

	// 衝突属性(自分)を取得
	uint32_t GetTypeId() const { return typeID_; }

	// 衝突判定の有効/無効を取得
	bool IsCollisionEnabled() const { return isCollisionEnabled_; }

public:	// setter
	// 半径を設定
	void SetRadius(float radius) { radius_ = radius; }
	// AABBを設定
	void SetAABB(const AABB& aabb) { aabb_ = aabb; }
	// OBBを設定
	void SetOBB(const OBB& obb) { obb_ = obb; }

	// 衝突属性(自分)を設定
	void SetTypeId(uint32_t typeId) { typeID_ = typeId; }

	// 衝突判定の有効/無効を設定
	void SetCollisionEnabled(bool isEnabled) { isCollisionEnabled_ = isEnabled; }

private:	// メンバ変数
	// 半径
	float radius_ = 1.0f;
	// AABB
	AABB aabb_ = { Vector3{0, 0, 0}, Vector3{1, 1, 1} }; // AABBの初期値は適当な値を設定
	// OBB
	OBB obb_ = { Vector3{0, 0, 0}, {Vector3{0, 0, 0}, Vector3{0, 0, 0},Vector3{0, 0, 0} }, Vector3{1, 1, 1} }; // OBBの初期値は適当な値を設定

	// 衝突属性
	uint32_t typeID_ = 0u;

	bool isCollisionEnabled_ = true; // 衝突判定を有効にするかどうか
};

