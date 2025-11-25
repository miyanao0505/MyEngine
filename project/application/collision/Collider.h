#pragma once
#include "MyBase.h"
#include "Quaternion.h"

/// <summary>
/// Colliderクラス
/// ゲームオブジェクトに付与される衝突判定用コンポーネントの基底クラス。 
/// </summary>
class Collider
{
public:	// メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Collider();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Collider();

	/// <summary>
	/// 衝突時のコールバック関数
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	virtual void OnCollision([[maybe_unused]] Collider* other) {};

public:	// gettere
	/// <summary>
	/// 半径を取得
	/// </summary>
	/// <returns>半径</returns>
	float GetRadius() const { return radius_; }
	
	/// <summary>
	/// AABBを取得
	/// </summary>
	/// <returns>AABB</returns>
	const MyBase::AABB& GetAABB() const { return aabb_; }
	
	/// <summary>
	/// OBBを取得
	/// </summary>
	/// <returns>OBB</returns>
	const MyBase::OBB& GetOBB() const { return obb_; }
	
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	virtual MyBase::Vector3 GetWorldPosition() = 0;

	/// <summary>
	/// 衝突属性(自分)を取得
	/// </summary>
	/// <returns>衝突属性</returns>
	uint32_t GetTypeId() const { return typeID_; }

	/// <summary>
	/// 衝突判定の有効/無効を取得
	/// </summary>
	/// <returns>衝突判定の有効/無効</returns>
	bool IsCollisionEnabled() const { return isCollisionEnabled_; }

public:	// setter
	/// <summary>
	/// 半径を設定
	/// </summary>
	/// <param name="radius">半径</param>
	void SetRadius(float radius) { radius_ = radius; }
	
	/// <summary>
	/// AABBを設定
	/// </summary>
	/// <param name="aabb">AABB</param>
	void SetAABB(const MyBase::AABB& aabb) { aabb_ = aabb; }
	
	/// <summary>
	/// OBBを設定
	/// </summary>
	/// <param name="obb">OBB</param>
	void SetOBB(const MyBase::OBB& obb) { obb_ = obb; }

	/// <summary>
	/// 衝突属性(自分)を設定
	/// </summary>
	/// <param name="typeId">衝突属性</param>
	void SetTypeId(uint32_t typeId) { typeID_ = typeId; }

	/// <summary>
	/// 衝突判定の有効/無効を設定
	/// </summary>
	/// <param name="isEnabled">衝突判定の有効/無効</param>
	void SetCollisionEnabled(bool isEnabled) { isCollisionEnabled_ = isEnabled; }

private:	// メンバ変数
	// 半径
	float radius_ = 1.0f;
	// AABB
	MyBase::AABB aabb_ = { MyBase::Vector3{0, 0, 0}, MyBase::Vector3{1, 1, 1} }; // AABBの初期値は適当な値を設定
	// OBB
	MyBase::OBB obb_ = { MyBase::Vector3{0, 0, 0}, {MyBase::Vector3{0, 0, 0}, MyBase::Vector3{0, 0, 0},MyBase::Vector3{0, 0, 0} }, MyBase::Vector3{1, 1, 1} }; // OBBの初期値は適当な値を設定

	// 衝突属性
	uint32_t typeID_ = 0u;

	bool isCollisionEnabled_ = true; // 衝突判定を有効にするかどうか
};

