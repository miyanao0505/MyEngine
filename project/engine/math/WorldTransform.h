#pragma once
#include "MyBase.h"
#include "Matrix.h"

/// <summary>
/// ワールドトランスフォーム管理クラス
/// </summary>
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
	/// <summary>
	/// 位置を設定
	/// </summary>
	/// <param name="transform">新しい位置ベクトル</param>
	void SetTranslate(const MyBase::Vector3& transform) { transform_.translate = transform; MarkDirty(); }
	
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rotate">新しい回転(Euler角)</param>
	void SetRotate(const MyBase::Vector3& rotate) { transform_.rotate = rotate; MarkDirty(); }
	
	/// <summary>
	/// スケールを設定
	/// </summary>
	/// <param name="scale">新しいスケール</param>
	void SetScale(const MyBase::Vector3& scale) { transform_.scale = scale; MarkDirty(); }
	
	/// <summary>
	/// Transform 構造体全体を設定
	/// </summary>
	/// <param name="transform">位置・回転・スケールをまとめた Transform 構造体</param>
	void SetTransform(const MyBase::Transform& transform) { transform_ = transform; MarkDirty(); }
	
	/// <summary>
	/// 親ワールドトランスフォームを設定
	/// </summary>
	/// <param name="parent">親となる WorldTransform へのポインタ</param>
	void SetParent(WorldTransform* parent) { parent_ = parent; MarkDirty(); }

public:	// getter
	/// <summary>
	/// 位置ベクトルを取得
	/// </summary>
	/// <returns>位置ベクトル</returns>
	const MyBase::Vector3& GetTranslate() const { return transform_.translate; }
	
	/// <summary>
	/// 回転ベクトル(Euler角)を取得
	/// </summary>
	/// <returns>回転ベクトル(Euler角)</returns>
	const MyBase::Vector3& GetRotate() const { return transform_.rotate; }
	
	/// <summary>
	/// スケールベクトルを取得
	/// </summary>
	/// <returns>スケールベクトル</returns>
	const MyBase::Vector3& GetScale() const { return transform_.scale; }
	
	/// <summary>
	/// Transform 構造体を取得
	/// </summary>
	/// <returns>Transform 構造体</returns>
	const MyBase::Transform& GetTransform() const { return transform_; }
	
	/// <summary>
	/// ワールド行列を取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const MyBase::Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }
	
	/// <summary>
	/// 親ワールドトランスフォームを取得する。
	/// </summary>
	/// <returns>親ワールドトランスフォームへのポインタ</returns>
	WorldTransform* GetParent() const { return parent_; }

private:	// メンバ変数
	MyBase::Transform transform_;

	MyBase::Matrix4x4 worldMatrix_ = Matrix::MakeIdentity4x4();	// ワールド行列

	WorldTransform* parent_ = nullptr;	// 親ワールドトランスフォーム

	bool isDirty_ = true;	// ワールド行列を更新する必要があるかどうかのフラグ

};