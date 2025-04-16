#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <assimp/scene.h>
#include "MyBase.h"

// 前方宣言
class ModelBase;

// 3Dモデル
class Model
{
public:	// メンバ関数
	// 初期化
	void Initialize(ModelBase* modelBase, const std::string& directorypath, const std::string& filename);
	// 描画処理
	void Draw();
	// .mtlファイルの読み取り
	static MyBase::MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	// .objファイルの読み取り
	void LoadObjFile(const std::string& directoryPath, const std::string& filename);

public:	// getter
	const MyBase::ModelData& GetModelData() const { return modelData_; }
	const bool& GetEnableLighting() const { return materialData_->enableLighting; }

public:	// setter
	void SetEnableLighting(const bool& enableLighting) { materialData_->enableLighting = enableLighting; }

private:	// メンバ関数
	// 頂点データ作成
	void CreateVertexData();
	// マテリアルデータ作成
	void CreateMaterialData();
	// Node情報を読み込む
	MyBase::Node ReadNode(aiNode* node);

private:	// メンバ変数
	ModelBase* modelBase_ = nullptr;

	// objファイルのデータ
	MyBase::ModelData modelData_;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;				// vertex
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;				// マテリアル

	// バッファリソース内のデータを指すポインタ
	MyBase::ModelVertexData* vertexData_ = nullptr;										// vertex
	MyBase::ModelMaterial* materialData_ = nullptr;									// マテリアル

	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};									// vertex

};

