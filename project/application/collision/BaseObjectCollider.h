#pragma once
#include "Collider.h"
#include "BaseObject.h"

class BaseObjectCollider : public Collider
{
public:	// メンバ関数
	explicit BaseObjectCollider(BaseObject* owner) : owner_(owner) {}

	// 衝突時のコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override { if (owner_) { owner_->OnCollision(other); } };

public:	// getter
	// ワールド座標を取得
	Vector3 GetWorldPosition() override { return owner_ ? owner_->GetWorldPosition() : Vector3{ 0,0,0 }; }

private: // メンバ変数
	BaseObject* owner_ = nullptr;	// 所有者のBaseObject
};

