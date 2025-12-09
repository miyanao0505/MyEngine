#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include "Model.h"
#include "MyBase.h"
#include "WorldTransform.h"

// 前方宣言
class Object3dBase;

/// <summary>
/// ゲームシーン上に配置される 3D オブジェクトを表すクラス。
/// Object3d クラスは、3D モデル(Model クラス)の描画を行うための個別オブジェクトを管理します。
/// </summary>
class Object3d
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="filePath">読み込むモデルファイルのパス</param>
	void Initialize(const std::string& filePath);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:	// getter
	/// <summary>
	/// 使用しているモデルを取得
	/// </summary>
	/// <returns>Model クラスのポインタ</returns>
	Model* GetModel() const { return model_; }
	
	/// <summary>
	/// オブジェクトのスケール値を取得
	/// </summary>
	/// <returns>スケールベクトル (X, Y, Z) への参照</returns>
	const MyBase::Vector3& GetScale() const { return worldTransform_->GetScale(); }
	
	/// <summary>
	/// オブジェクトの回転値を取得
	/// </summary>
	/// <returns>回転ベクトル (X, Y, Z) への参照</returns>
	const MyBase::Vector3& GetRotate() const { return worldTransform_->GetRotate(); }
	
	/// <summary>
	/// オブジェクトの位置(平行移動)を取得
	/// </summary>
	/// <returns>位置ベクトル (X, Y, Z) への参照</returns>
	const MyBase::Vector3& GetTranslate() const { return worldTransform_->GetTranslate(); }
	
	/// <summary>
	/// ワールド変換クラスの取得
	/// </summary>
	/// <returns>WorldTransform クラス</returns>
	WorldTransform* GetWorldTransform() const { return worldTransform_.get(); }
	
	/// <summary>
	/// ライティングの有効状態を取得
	/// </summary>
	/// <returns>有効なら 1、無効なら 0</returns>
	const int GetEnableLighting() const { return model_->GetEnableLighting(); }

public:	// setter
	/// <summary>
	/// モデルを指定して設定
	/// </summary>
	/// <param name="model">設定するモデル</param>
	void SetModel(Model* model) { model_ = model; }
	
	/// <summary>
	/// モデルファイルを指定して読み込み・設定
	/// </summary>
	/// <param name="filePath">読み込むモデルファイルパス</param>
	void SetModel(const std::string& filePath);
	
	/// <summary>
	/// 描画に使用するテクスチャを変更
	/// </summary>
	/// <param name="filename">テクスチャファイル名</param>
	void SetTexture(const std::string& filename);
	
	/// <summary>
	/// スケール値を設定
	/// </summary>
	/// <param name="scale">スケールベクトル (X, Y, Z)</param>
	void SetScale(const MyBase::Vector3& scale) { worldTransform_->SetScale(scale); }
	
	/// <summary>
	/// 回転値を設定
	/// </summary>
	/// <param name="rotate">回転ベクトル (X, Y, Z)</param>
	void SetRotate(const MyBase::Vector3& rotate) { worldTransform_->SetRotate(rotate); }
	
	/// <summary>
	/// 座標値を設定
	/// </summary>
	/// <param name="translate">位置ベクトル (X, Y, Z)</param>
	void SetTranslate(const MyBase::Vector3& translate) { worldTransform_->SetTranslate(translate); }
	
	/// <summary>
	/// Transform を設定
	/// </summary>
	/// <param name="transform">新しい Transform</param>
	void SetTransform(const MyBase::Transform& transform) { worldTransform_->SetScale(transform.scale); worldTransform_->SetRotate(transform.rotate); worldTransform_->SetTranslate(transform.translate); }
	
	/// <summary>
	/// 外部の WorldTransform をコピーして設定
	/// </summary>
	/// <param name="worldTransform">設定するワールドトランスフォーム</param>
	void SetWorldTransform(const WorldTransform& worldTransform) { *worldTransform_ = worldTransform; }
	
	/// <summary>
	/// ライティングの有効/無効を設定
	/// </summary>
	/// <param name="enableLighting">true でライティング有効、false で無効</param>
	void SetEnableLighting(bool enableLighting) { model_->SetEnableLighting(enableLighting); }

private:	// メンバ関数
	/// <summary>
	/// 座標変換行列データを GPU に送信するためのリソースを生成
	/// </summary>
	void CreateTransformationMatrixData();
	
	/// <summary>
	/// カメラ情報を GPU に送信するためのリソースを生成
	/// </summary>
	void CreateCameraData();

private:	// メンバ変数
	Object3dBase* object3dBase_ = nullptr;

	Model* model_ = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;	// 座標変換行列
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_ = nullptr;				// カメラ
	// バッファリソース内のデータを指すポインタ
	MyBase::TransformationMatrix* transformationMatrixData_ = nullptr;				// 座標変換行列
	MyBase::CameraForGPU* cameraData_ = nullptr;									// カメラ

	// WorldTransform
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;		// ワールドトランスフォーム

	// Texture
	std::string textureFileName_;
};

