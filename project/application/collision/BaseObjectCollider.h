#pragma once
#include "Collider.h"
#include "BaseObject.h"

/// <summary>
/// BaseObjectColliderクラス
/// BaseObjectに関連付けられたコライダーを表すクラス。
/// </summary>
class BaseObjectCollider : public Collider
{
public:	// メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のBaseObjectへのポインタ</param>
	explicit BaseObjectCollider(BaseObject* owner) : owner_(owner) {}

	/// <summary>
	/// 衝突時のコールバック関数
	/// </summary>
	/// <param name="other">衝突相手の Collider</param>
	void OnCollision([[maybe_unused]] Collider* other) override { if (owner_) { owner_->OnCollision(other); } };

public:	// getter
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標(Vector3)</returns>
	MyBase::Vector3 GetWorldPosition() const override { return owner_ ? owner_->GetWorldPosition() : MyBase::Vector3{ 0.0f, 0.0f, 0.0f }; }

private: // メンバ変数
	BaseObject* owner_ = nullptr;	// 所有者のBaseObject
};