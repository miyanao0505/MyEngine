#pragma once
#include "MyBase.h"
#include "Matrix.h"

class WorldTransform
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ワールド行列を更新する必要があることをマーク
	/// </summary>
	void MarkDirty() { isDirty_ = true; }

	/// <summary>
	/// ワールド行列を更新
	/// </summary>
	void UpdateWorldMatrix();

public:	// setter
	void SetTranslate(const MyBase::Vector3& transform) { transform_.translate = transform; MarkDirty(); }
	void SetRotate(const MyBase::Vector3& rotate) { transform_.rotate = rotate; MarkDirty(); }
	void SetScale(const MyBase::Vector3& scale) { transform_.scale = scale; MarkDirty(); }
	void SetTransform(const MyBase::Transform& transform) { transform_ = transform; MarkDirty(); }
	void SetParent(WorldTransform* parent) { parent_ = parent; MarkDirty(); }

public:	// getter
	const MyBase::Vector3& GetTranslate() const { return transform_.translate; }	// 位置を取得
	const MyBase::Vector3& GetRotate() const { return transform_.rotate; }			// 回転を取得
	const MyBase::Vector3& GetScale() const { return transform_.scale; }			// 拡大縮小を取得
	const MyBase::Transform& GetTransform() const { return transform_; }			// Transformを取得
	const MyBase::Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }	// ワールド行列を取得
	WorldTransform* GetParent() const { return parent_; }						// 親ワールドトランスフォームを取得

private:	// メンバ変数
	MyBase::Transform transform_;

	MyBase::Matrix4x4 worldMatrix_ = Matrix::MakeIdentity4x4();	// ワールド行列

	WorldTransform* parent_ = nullptr;	// 親ワールドトランスフォーム

	bool isDirty_ = true;	// ワールド行列を更新する必要があるかどうかのフラグ

};

