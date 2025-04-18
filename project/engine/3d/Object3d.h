#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Model.h"
#include "MyBase.h"

// 前方宣言
class Object3dBase;

// 3Dオブジェクト
class Object3d
{
public:	// メンバ関数
	// 初期化
	void Initislize();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

public:	// getter
	const MyBase::Vector3& GetScale() const { return transform_.scale; }
	const MyBase::Vector3& GetRotate() const { return transform_.rotate; }
	const MyBase::Vector3& GetTranslate() const { return transform_.translate; }
	const int& GetEnableLighting() const { return model_->GetEnableLighting(); }

public:	// setter
	void SetModel(Model* model) { model_ = model; }
	void SetModel(const std::string& filePath);
	void SetScale(const MyBase::Vector3& scale) { transform_.scale = scale; }
	void SetRotate(const MyBase::Vector3& rotate) { transform_.rotate = rotate; }
	void SetTranslate(const MyBase::Vector3& translate) { transform_.translate = translate; }
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

	// Transform
	MyBase::Transform transform_;			// 3Dオブジェクト
};

