#pragma once
#include <memory>
#include "Object3d.h"
#include "Collider.h"
#include "MyBase.h"

class BaseObject
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

#ifdef _DEBUG
	/// <summary>
	/// デバック描画
	/// </summary>
	virtual void DebugDraw();
#endif // _DEBUG

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
	virtual MyBase::Vector3 GetWorldPosition() { return object_ ? object_->GetTranslate() : MyBase::Vector3{ 0,0,0 }; }
	// ---- Collider連携 ----

	Collider* GetCollider() const { return collider_.get(); }
	Object3d* GetObject3d() const { return object_.get(); }
	std::string GetName() const { return name_; }

public:	// setter
	void SetModel(const std::string& modelPath) { if (object_) { object_->SetModel(modelPath); } }
	void SetObject3d(std::unique_ptr<Object3d> object) { object_ = std::move(object); }
	void SetCollider(std::unique_ptr<Collider> collider) { collider_ = std::move(collider); }
	void SetName(const std::string& name) { name_ = name; }

protected:	// メンバ変数
	std::unique_ptr<Object3d> object_ = nullptr;	// モデルデータ	
	std::unique_ptr<Collider> collider_ = nullptr;	// 衝突判定用コライダー

	std::string name_;								// オブジェクト名

};

