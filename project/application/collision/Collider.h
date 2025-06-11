#pragma once
#include "MyBase.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider : public MyBase
{
public:	// メンバ関数
	// 衝突時に呼ばれる関数
	virtual void OnCollision();

public:	// gettere
	// 半径を取得
	float GetRadius() const { return radius_; }
	// サイズを取得
	const Vector2& GetSize() const { return size_; }
	// ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;

	// 衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() const { return collisionAttribute_; }
	// 衝突マスク(相手)を取得
	uint32_t GetCollisionMask() const { return collisionMask_; }

public:	// setter
	// 半径を設定
	void SetRadius(float radius) { radius_ = radius; }
	// サイズを設定
	void SetSize(const Vector2& size) { size_ = size; }

	// 衝突属性(自分)を設定
	void SetCollisionAttribute(uint32_t attribute) { collisionAttribute_ = attribute; }
	// 衝突マスク(相手)を設定
	void SetCollisionMask(uint32_t mask) { collisionMask_ = mask; }

private:	// メンバ変数
	// 半径
	float radius_ = 1.0f;
	// サイズ
	Vector2 size_ = { 1.0f, 1.0f };

	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;
};

