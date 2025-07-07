#pragma once
#include "MyBase.h"


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
	// サイズを取得
	const Vector2& GetSize() const { return size_; }
	// ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;

	// 衝突属性(自分)を取得
	uint32_t GetTypeId() const { return typeID_; }

	// 衝突判定の有効/無効を取得
	bool IsCollisionEnabled() const { return isCollisionEnabled_; }

public:	// setter
	// 半径を設定
	void SetRadius(float radius) { radius_ = radius; }
	// サイズを設定
	void SetSize(const Vector2& size) { size_ = size; }

	// 衝突属性(自分)を設定
	void SetTypeId(uint32_t typeId) { typeID_ = typeId; }

	// 衝突判定の有効/無効を設定
	void SetCollisionEnabled(bool isEnabled) { isCollisionEnabled_ = isEnabled; }

private:	// メンバ変数
	// 半径
	float radius_ = 1.0f;
	// サイズ
	Vector2 size_ = { 1.0f, 1.0f };

	// 衝突属性
	uint32_t typeID_ = 0u;


	bool isCollisionEnabled_ = true; // 衝突判定を有効にするかどうか
};

