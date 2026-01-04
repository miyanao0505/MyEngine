#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <assimp/scene.h>
#include "MyBase.h"

// 前方宣言
class ModelBase;

/// <summary>
/// 3Dモデルデータを管理および描画するためのクラス。
/// </summary>
class Model
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelBase">基底となる ModelBase インスタンスへのポインタ</param>
	/// <param name="directoryPath">モデルファイルが格納されているディレクトリパス</param>
	/// <param name="fileName">読み込むモデルファイル名</param>
	void Initialize(ModelBase* modelBase, const std::string& directoryPath, const std::string& fileName);
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
	/// <summary>
	/// .mtl(マテリアル)ファイルを読み込み、MaterialData を生成
	/// </summary>
	/// <param name="directoryPath">mtl ファイルが存在するディレクトリパス</param>
	/// <param name="fileName">読み込む mtl ファイル名</param>
	/// <returns>MaterialData 構造体</returns>
	static MyBase::MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName);
	
	/// <summary>
	/// .obj ファイルを読み込み、モデルデータを構築
	/// </summary>
	/// <param name="directoryPath">obj ファイルが存在するディレクトリパス</param>
	/// <param name="fileName">読み込む obj ファイル名</param>
	void LoadObjFile(const std::string& directoryPath, const std::string& fileName);

public:	// getter
	/// <summary>
	/// モデルデータを取得
	/// </summary>
	/// <returns>ModelData への参照</returns>
	const MyBase::ModelData& GetModelData() const { return modelData_; }
	
	/// <summary>
	/// モデルのテクスチャファイル名を取得
	/// </summary>
	/// <returns>テクスチャファイル名への参照</returns>
	const std::string& GetTexture() const { return modelData_.material.textureFilePath; }
	
	/// <summary>
	/// モデルのマテリアル情報を取得
	/// </summary>
	/// <returns>ModelMaterial へのポインタ。マテリアル未設定の場合は nullptr</returns>
	MyBase::ModelMaterial* GetModelMaterial() const { return (materialDataPtr_ == nullptr) ? nullptr : materialDataPtr_; }
	
	/// <summary>
	/// ライティングの有効状態を取得
	/// </summary>
	/// <returns>有効なら 1、無効なら 0</returns>
	bool GetEnableLighting() const { return materialDataPtr_->enableLighting; }

public:	// setter
	/// <summary>
	/// ライティングの有効/無効を設定
	/// </summary>
	/// <param name="enableLighting">true でライティング有効、false で無効</param>
	void SetEnableLighting(bool enableLighting) { materialDataPtr_->enableLighting = enableLighting; }
	
	/// <summary>
	/// モデルに使用するテクスチャを設定
	/// </summary>
	/// <param name="fileName">設定するテクスチャファイル名</param>
	void SetTexture(const std::string& fileName);
	
	/// <summary>
	/// モデルのマテリアル情報を設定
	/// </summary>
	/// <param name="materialData">設定する ModelMaterial へのポインタ</param>
	void SetModelMaterial(MyBase::ModelMaterial* materialData) { materialDataPtr_ = materialData; }
	
	/// <summary>
	/// 環境マップ用のテクスチャ名を設定
	/// </summary>
	/// <param name="textureName">設定する環境テクスチャ名</param>
	void SetEnvironmentTexture(std::string textureName) { environmentTexturePath_ = textureName; }

private:	// メンバ関数
	/// <summary>
	/// 頂点データを作成
	/// </summary>
	void CreateVertexData();
	
	/// <summary>
	/// マテリアルデータを作成
	/// </summary>
	void CreateMaterialData();
	
	/// <summary>
	/// Assimp のノード情報を読み込み、Node 構造体に変換
	/// </summary>
	/// <param name="node">読み込む aiNode ポインタ</param>
	/// <returns>変換された Node 情報</returns>
	MyBase::Node ReadNode(aiNode* node);

private:	// メンバ変数
	ModelBase* modelBase_ = nullptr;

	// objファイルのデータ
	MyBase::ModelData modelData_;
	// 環境光用のテクスチャ
	std::string environmentTexturePath_;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;				// vertex
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;				// マテリアル

	// バッファリソース内のデータを指すポインタ
	MyBase::ModelVertexData* vertexDataPtr_ = nullptr;								// vertex
	MyBase::ModelMaterial* materialDataPtr_ = nullptr;								// マテリアル

	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};									// vertex

#pragma region 定数
private:
	static constexpr uint32_t kRootParamMaterialCBV = 0;	// ルートパラメータ：マテリアル用 CBV
	static constexpr uint32_t kRootParamMainTexture = 2;	// ルートパラメータ：メインテクスチャ SRV
	static constexpr uint32_t kRootParamEnviromentTexture = 7;	// ルートパラメータ：環境テクスチャ SRV

	static constexpr uint32_t kDefaultInstanceCount = 1;
	static constexpr uint32_t kVertexStartOffset = 0;
	static constexpr uint32_t kInstanceStartOffset = 0;

	static constexpr uint32_t kTriangleVertexCount = 3;
	static constexpr uint32_t kMatrixSize = 4;

	static constexpr float kPositionW = 1.0f;
	static constexpr float kLeftHandedFlipSign = -1.0f;

	static const MyBase::Vector4 kDefaultMaterialColor;
	static const float kDefaultShininess;
	static const float kDefaultReflectivity;
#pragma endregion
};