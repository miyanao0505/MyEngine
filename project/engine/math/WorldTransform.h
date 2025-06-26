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
	void SetPosition(const MyBase::Vector3& position) { position_ = position; MarkDirty(); }
	void SetRotation(const MyBase::Vector3& rotation) { rotation_ = rotation; MarkDirty(); }
	void SetScale(const MyBase::Vector3& scale) { scale_ = scale; MarkDirty(); }
	void SetParent(WorldTransform* parent) { parent_ = parent; MarkDirty(); }

public:	// getter
	const MyBase::Vector3& GetPosition() const { return position_; }	// 位置を取得
	const MyBase::Vector3& GetRotation() const { return rotation_; }	// 回転を取得
	const MyBase::Vector3& GetScale() const { return scale_; }		// 拡大縮小を取得
	const MyBase::Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }	// ワールド行列を取得
	WorldTransform* GetParent() const { return parent_; }	// 親ワールドトランスフォームを取得

private:	// メンバ変数
	MyBase::Vector3 position_{0.0f, 0.0f, 0.0f};		// 位置
	MyBase::Vector3 rotation_{0.0f, 0.0f, 0.0f};		// 回転
	MyBase::Vector3 scale_{1.0f, 1.0f, 1.0f};			// 拡大縮小

	MyBase::Matrix4x4 worldMatrix_ = Matrix::MakeIdentity4x4();	// ワールド行列

	WorldTransform* parent_ = nullptr;	// 親ワールドトランスフォーム

	bool isDirty_ = true;	// ワールド行列を更新する必要があるかどうかのフラグ

};

