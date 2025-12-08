#pragma once
#include <MyBase.h>

/// <summary>
/// 3Dシーンの視点(カメラ)を管理するクラス。
/// </summary>
class Camera
{
public:	// メンバ関数
	/// <summary>
	/// Camera クラスのコンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

public:	// getter
	/// <summary>
	/// ワールド行列を取得
	/// </summary>
	/// <returns>カメラのワールド行列への参照</returns>
	const MyBase::Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }

	/// <summary>
	/// ビュー行列を取得
	/// </summary>
	/// <returns>カメラのビュー行列への参照</returns>
	const MyBase::Matrix4x4& GetViewMatrix() const { return viewMatrix_; }

	/// <summary>
	/// プロジェクション行列を取得
	/// </summary>
	/// <returns>カメラのプロジェクション行列への参照</returns>
	const MyBase::Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
	
	/// <summary>
	/// ビュー行列とプロジェクション行列を合成した行列を取得
	/// </summary>
	/// <returns>ビュー・プロジェクション行列への参照</returns>
	const MyBase::Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
	
	/// <summary>
	/// カメラの Transform を取得
	/// </summary>
	/// <returns>Transform への参照</returns>
	const MyBase::Transform& GetTransform() const { return transform_; }
	
	/// <summary>
	/// カメラの回転値を取得
	/// </summary>
	/// <returns>回転ベクトル (X, Y, Z) への参照</returns>
	const MyBase::Vector3& GetRotate() const { return transform_.rotate; }
	
	/// <summary>
	/// カメラの位置(平行移動)を取得
	/// </summary>
	/// <returns>位置ベクトル (X, Y, Z) への参照</returns>
	const MyBase::Vector3& GetTranslate() const { return transform_.translate; }
	
	/// <summary>
	/// 垂直方向の視野角を取得
	/// </summary>
	/// <returns>視野角(ラジアン)への参照</returns>
	const float& GetFovY() const { return fovY_; }

	/// <summary>
	///  アスペクト比を取得
	/// </summary>
	/// <returns>画面幅 / 画面高さの比率への参照</returns>
	const float& GetAspectRatio() const { return aspectRatio_; }

	/// <summary>
	/// ニアクリップ距離を取得
	/// </summary>
	/// <returns>ニアクリップ距離への参照</returns>
	const float& GetNearCip() const { return nearClip_; }

	/// <summary>
	/// ファークリップ距離を取得
	/// </summary>
	/// <returns>ファークリップ距離への参照</returns>
	const float& GetFarClip() const { return farClip_; }

public:	// setter
	/// <summary>
	/// カメラの Transform を設定
	/// </summary>
	/// <param name="transform">新しい Transform</param>
	void SetTransform(const MyBase::Transform& transform) { transform_ = transform; }
	
	/// <summary>
	/// カメラの回転値を設定
	/// </summary>
	/// <param name="rotate">回転ベクトル (X, Y, Z)</param>
	void SetRotate(const MyBase::Vector3& rotate) { transform_.rotate = rotate; }
	
	/// <summary>
	/// カメラの位置(平行移動)を設定
	/// </summary>
	/// <param name="translate">位置ベクトル (X, Y, Z)</param>
	void SetTranslate(const MyBase::Vector3& translate) { transform_.translate = translate; };
	
	/// <summary>
	/// 垂直方向の視野角を設定
	/// </summary>
	/// <param name="fovY">視野角(ラジアン)</param>
	void SetFovY(const float& fovY) { fovY_ = fovY; }
	
	/// <summary>
	/// アスペクト比を設定
	/// </summary>
	/// <param name="aspectRatio">画面幅 / 画面高さの比率</param>
	void SetAspectRatio(const float& aspectRatio) { aspectRatio_ = aspectRatio; }
	
	/// <summary>
	/// ニアクリップ距離を設定
	/// </summary>
	/// <param name="nearClip">ニアクリップ距離</param>
	void SetNearClip(const float& nearClip) { nearClip_ = nearClip; }
	
	/// <summary>
	/// ファークリップ距離を設定
	/// </summary>
	/// <param name="farClip">ファークリップ距離</param>
	void SetFarClip(const float& farClip) { farClip_ = farClip; }

protected:	// メンバ変数
	// ビュー関連データ
	MyBase::Transform transform_;	// カメラの位置・回転情報
	MyBase::Matrix4x4 worldMatrix_;	// ワールド行列
	MyBase::Matrix4x4 viewMatrix_;	// ビュー行列

	// プロジェクション行列関連データ
	MyBase::Matrix4x4 projectionMatrix_;	// プロジェクション行列
	float fovY_;							// 垂直方向視野角
	float aspectRatio_;						// アスペクト比
	float nearClip_;						// ニアクリップ距離
	float farClip_;							// ファークリップ距離

	// 合成行列
	MyBase::Matrix4x4 viewProjectionMatrix_;

};

