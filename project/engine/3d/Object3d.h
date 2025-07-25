#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include "Model.h"
#include "MyBase.h"
#include "WorldTransform.h"


// 前方宣言
class Object3dBase;

// 3Dオブジェクト
class Object3d
{
public:	// メンバ関数
	// 初期化
	void Initislize(const std::string& filePath);
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

public:	// getter
	Model* GetModel() const { return model_; }
	const MyBase::Vector3& GetScale() const { return worldTransform_->GetScale(); }
	const MyBase::Vector3& GetRotate() const { return worldTransform_->GetRotation(); }
	const MyBase::Vector3& GetTranslate() const { return worldTransform_->GetPosition(); }
	WorldTransform* GetWorldTransform() const { return worldTransform_.get(); }
	const int& GetEnableLighting() const { return model_->GetEnableLighting(); }

public:	// setter
	void SetModel(Model* model) { model_ = model; }
	void SetModel(const std::string& filePath);
	void SetTexture(const std::string& filename);
	void SetScale(const MyBase::Vector3& scale) { worldTransform_->SetScale(scale); }
	void SetRotate(const MyBase::Vector3& rotate) { worldTransform_->SetRotation(rotate); }
	void SetTranslate(const MyBase::Vector3& translate) { worldTransform_->SetPosition(translate); }
	void SetTransform(const MyBase::Transform& transform) { worldTransform_->SetScale(transform.scale); worldTransform_->SetRotation(transform.rotate); worldTransform_->SetPosition(transform.translate); }
	void SetWorldTransform(const WorldTransform& worldTransform) { *worldTransform_ = worldTransform; }
	void SetEnableLighting(const bool& enableLighting) { model_->SetEnableLighting(enableLighting); }

private:	// メンバ関数
	// 座標変換行列データ作成
	void CreateTransformationMatrixData();
	// カメラデータ作成
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

