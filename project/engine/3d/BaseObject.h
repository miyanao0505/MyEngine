#pragma once
#include <memory>
#include "Object3d.h"
#include "Collider.h"
#include "MyBase.h"

class BaseObject : public MyBase
{
public:	// メンバ関数
	BaseObject() = default;
	virtual ~BaseObject() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& modelPath);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 衝突判定処理
	/// </summary>
	/// <param name="other"></param>
	virtual void OnCollision([[maybe_unused]] Collider* other);

public:	// getter

	// ---- Collider連携 ----
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	virtual Vector3 GetWorldPosition() { return object_ ? object_->GetTranslate() : Vector3{ 0,0,0 }; }
	// ---- Collider連携 ----

	Collider* GetCollider() const { return collider_.get(); }
	Object3d* GetObject3d() const { return object_.get(); }

public:	// setter
	void SetCollider(std::unique_ptr<Collider> collider) { collider_ = std::move(collider); }

protected:	// メンバ変数
	std::unique_ptr<Object3d> object_ = nullptr;		// モデルデータ	
	std::unique_ptr<Collider> collider_ = nullptr;	// 衝突判定用コライダー

};

