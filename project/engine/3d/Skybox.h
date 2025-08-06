#pragma once
#include "DirectXBase.h"
#include "WorldTransform.h"
#include "MyBase.h"

class Skybox
{
public:	// メンバ関数
	// 初期化
	void Initislize(const std::string& filePath, MyBase::Vector3 scale);
	// 更新処理
	void Update();
	// 描画処理
	void Draw();
	// デバック描画
	void DebugDraw();

public:	// getter
	DirectXBase* GetDxBase() const { return dxBase_; }
	WorldTransform* GetWorldTransform() const { return worldTransform_.get(); }
	MyBase::Transform GetTransform() const { return MyBase::Transform{ worldTransform_->GetScale(), worldTransform_->GetRotate(), worldTransform_->GetTranslate() }; }
	MyBase::Vector3 GetScale() const { return worldTransform_->GetScale(); }
	MyBase::Vector3 GetRotate() const { return worldTransform_->GetRotate(); }
	MyBase::Vector3 GetTranslate() const { return worldTransform_->GetTranslate(); }
	const std::string& GetTextureName() { return textureFileName_; }
	bool IsUsingEnvironmentMap() const { return useEnvironmentMap_; }

public:	// setter
	void SetWorldTransform(const WorldTransform& worldTransform) { *worldTransform_ = worldTransform; }
	void SetTransform(const MyBase::Transform& transform) { worldTransform_->SetScale(transform.scale); worldTransform_->SetRotate(transform.rotate); worldTransform_->SetTranslate(transform.translate); }
	void SetScale(const MyBase::Vector3& scale) { worldTransform_->SetScale(scale); }
	void SetRotate(const MyBase::Vector3& rotate) { worldTransform_->SetRotate(rotate); }
	void SetTranslate(const MyBase::Vector3& translate) { worldTransform_->SetTranslate(translate); }
	void SetTexture(const std::string& textureName);
	void SetUseEnvironmentMap(bool use) { useEnvironmentMap_ = use; }

private:	// メンバ関数
	// ルートシグネチャの作成
	void CreateRootSignature();
	// グラフィックスパイプラインの生成
	void CreateGraphicsPipeline();
	// 共通画面設定
	void SetCommonScreen();
	// 座標変換行列データ作成
	void CreateTransformationMatrixData();
	// カメラデータ作成
	void CreateCameraData();
	// 頂点データ作成
	void CreateVertexData();
	// インデックスデータ作成
	void CreateIndexData();
	// マテリアルデータ作成
	void CreateMaterialData();

private:	// メンバ変数
	// DirectXBase
	DirectXBase* dxBase_ = nullptr;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;
	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;	// 座標変換行列
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_ = nullptr;				// カメラ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;				// vertex
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;				// index
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;				// マテリアル
	// バッファリソース内のデータを指すポインタ
	MyBase::TransformationMatrix* transformationMatrixData_ = nullptr;				// 座標変換行列
	MyBase::CameraForGPU* cameraData_ = nullptr;									// カメラ
	MyBase::ModelSkyboxVertexData* vertexData_ = nullptr;							// vertex
	uint32_t* indexData_ = nullptr;													// index
	MyBase::ModelMaterial* materialData_ = nullptr;									// マテリアル

	// 頂点数
	static const size_t kVertexCount = 24;	// Skyboxの頂点数
	// インデックス数
	static const size_t kIndexCount = 36;	// Skyboxのインデックス数

	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	// WorldTransform
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;		// ワールドトランスフォーム

	// Texture
	std::string textureFileName_;

	// 使用フラグ
	bool useEnvironmentMap_ = false;	// デフォルトOFF

};

