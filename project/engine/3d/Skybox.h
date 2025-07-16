#pragma once
#include <DirectXBase.h>
#include <WorldTransform.h>


class Skybox
{
public:	// メンバ関数
	// 初期化
	void Initislize(DirectXBase* dxBase, const std::string& filePath);
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

public:	// getter
	DirectXBase* GetDxBase() const { return dxBase_; }

public:	// setter

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
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;				// マテリアル
	// バッファリソース内のデータを指すポインタ
	MyBase::TransformationMatrix* transformationMatrixData_ = nullptr;				// 座標変換行列
	MyBase::CameraForGPU* cameraData_ = nullptr;									// カメラ
	MyBase::ModelSkyboxVertexData* vertexData_ = nullptr;							// vertex
	MyBase::ModelMaterial* materialData_ = nullptr;									// マテリアル

	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	// WorldTransform
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;		// ワールドトランスフォーム

	// Texture
	std::string textureFileName_;
};

