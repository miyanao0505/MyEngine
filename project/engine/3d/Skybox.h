#pragma once
#include "DirectXBase.h"
#include "WorldTransform.h"
#include "MyBase.h"

enum class RootParam : UINT {
	kMaterial = 0,
	kTransform = 1,
	kTexture = 2,
	kCamera = 4,
};

/// <summary>
/// シーン全体を包み込む天空背景(スカイボックス)を描画するクラス
/// </summary>
class Skybox
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="filePath">使用するテクスチャのファイルパス</param>
	/// <param name="scale">Skybox の表示スケールベクトル (X, Y, Z)</param>
	void Initialize(const std::string& filePath, MyBase::Vector3 scale);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
#ifdef _DEBUG
	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw();
#endif // _DEBUG

public:	// getter
	/// <summary>
	/// 使用中の DirectXBase を取得
	/// </summary>
	/// <returns>DirectXBase へのポインタ</returns>
	DirectXBase* GetDxBase() const { return dxBase_; }
	
	/// <summary>
	/// ワールド変換クラスの取得
	/// </summary>
	/// <returns>WorldTransform クラス</returns>
	WorldTransform* GetWorldTransform() const { return worldTransform_.get(); }
	
	/// <summary>
	/// Skyboxの Transform を取得
	/// </summary>
	/// <returns>Transform</returns>
	MyBase::Transform GetTransform() const { return MyBase::Transform{ worldTransform_->GetScale(), worldTransform_->GetRotate(), worldTransform_->GetTranslate() }; }
	
	/// <summary>
	/// Skyboxのスケール値を取得
	/// </summary>
	/// <returns>スケールベクトル (X, Y, Z) への参照</returns>
	MyBase::Vector3 GetScale() const { return worldTransform_->GetScale(); }
	
	/// <summary>
	/// Skyboxの回転値を取得
	/// </summary>
	/// <returns>回転ベクトル (X, Y, Z) への参照</returns>
	MyBase::Vector3 GetRotate() const { return worldTransform_->GetRotate(); }
	
	/// <summary>
	/// Skyboxの位置(平行移動)を取得
	/// </summary>
	/// <returns>位置ベクトル (X, Y, Z) への参照</returns>
	MyBase::Vector3 GetTranslate() const { return worldTransform_->GetTranslate(); }
	
	/// <summary>
	/// Skyboxで使用しているテクスチャファイル名を取得
	/// </summary>
	/// <returns>テクスチャファイル名への参照</returns>
	const std::string& GetTextureFileName() { return textureFileName_; }
	
	/// <summary>
	/// 環境マップを使用しているかどうか取得
	/// </summary>
	/// <returns> 環境マップへの参照</returns>
	bool IsUsingEnvironmentMap() const { return useEnvironmentMap_; }

public:	// setter
	/// <summary>
	/// ワールド変換クラスの設定
	/// </summary>
	/// <param name="worldTransform">設定するワールドトランスフォーム</param>
	void SetWorldTransform(const WorldTransform& worldTransform) { *worldTransform_ = worldTransform; }
	
	/// <summary>
	/// Skyboxの Transform を設定
	/// </summary>
	/// <param name="transform">新しい Transform</param>
	void SetTransform(const MyBase::Transform& transform) { worldTransform_->SetScale(transform.scale); worldTransform_->SetRotate(transform.rotate); worldTransform_->SetTranslate(transform.translate); }
	
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
	/// 描画に使用するテクスチャを変更
	/// </summary>
	/// <param name="textureName">テクスチャファイル名</param>
	void SetTexture(const std::string& textureName);
	
	/// <summary>
	/// 環境マップの使用設定
	/// </summary>
	/// <param name="use">環境マップの使用フラグ</param>
	void SetUseEnvironmentMap(bool use) { useEnvironmentMap_ = use; }

private:	// メンバ関数
	/// <summary>
	/// ルートシグネチャを作成します。
	/// GPU がアクセスするリソースの配置定義を行います。
	/// </summary>
	void CreateRootSignature();
	
	/// <summary>
	/// Skybox 用のグラフィックスパイプラインを生成します。
	/// シェーダ・入力レイアウト・ブレンド設定などの構築を行います。
	/// </summary>
	void CreateGraphicsPipeline();
	
	/// <summary>
	/// 共通の画面設定（レンダーターゲット・ビューポート等）を適用します。
	/// Skybox 描画前に呼び出されます。
	/// </summary>
	void SetCommonScreen();
	
	/// <summary>
	/// 座標変換行列の GPU バッファを作成します。
	/// ワールド・ビュー・プロジェクション行列の転送準備を行います。
	/// </summary>
	void CreateTransformationMatrixData();
	
	/// <summary>
	/// カメラ情報の GPU バッファを作成します。
	/// カメラ位置やビュー行列を Skybox 用に転送できるよう準備します。
	/// </summary>
	void CreateCameraData();
	
	/// <summary>
	/// Skybox の頂点バッファを生成します。
	/// 立方体の全頂点の座標・UV などを GPU に転送します。
	/// </summary>
	void CreateVertexData();
	
	/// <summary>
	/// Skybox のインデックスバッファを生成します。
	/// 立方体の三角形構築に必要なインデックス情報を GPU に転送します。
	/// </summary>
	void CreateIndexData();
	
	/// <summary>
	/// Skybox のマテリアルデータを作成します。
	/// テクスチャ情報やライティング設定などを GPU に送る準備を行います。
	/// </summary>
	void CreateMaterialData();

private:	// メンバ変数
	// DirectXBase
	DirectXBase* dxBase_ = nullptr;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;
	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixBuffer_ = nullptr;	// 座標変換行列
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraBuffer_ = nullptr;				// カメラ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_ = nullptr;				// vertex
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer_ = nullptr;				// index
	Microsoft::WRL::ComPtr<ID3D12Resource> materialBuffer_ = nullptr;				// マテリアル
	// バッファリソース内のデータを指すポインタ
	MyBase::TransformationMatrix* transformationMatrixMapped_ = nullptr;				// 座標変換行列
	MyBase::CameraForGPU* cameraMapped_ = nullptr;									// カメラ
	MyBase::ModelSkyboxVertexData* vertexMapped_ = nullptr;							// vertex
	uint32_t* indexMapped_ = nullptr;													// index
	MyBase::ModelMaterial* materialMapped_ = nullptr;									// マテリアル

	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	// WorldTransform
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;		// ワールドトランスフォーム

	// Texture
	std::string textureFileName_;

	// 使用フラグ
	bool useEnvironmentMap_ = false;	// デフォルトOFF

#pragma region 定数
	static constexpr UINT kRootParamCount = 7;
	static constexpr UINT kDescriptorCount = 1;
	static constexpr UINT kSamplerCount = 1;
	static constexpr UINT kInputElementCount = 2;

	// 頂点数
	static const size_t kVertexCount;	// Skyboxの頂点数
	// インデックス数
	static const size_t kIndexCount;	// Skyboxのインデックス数

#ifdef _DEBUG
	static const float kPi;
	static const float kImGuiDragSpeed;				// ImGui のドラッグ速度	
	static const MyBase::ScopeF kTranslateScope;	// 平行移動の範囲
	static const MyBase::ScopeF kRotateScope;		// 回転の範囲
	static const MyBase::ScopeF kScaleScope;		// スケールの範囲
#endif // _DEBUG

	static const float kDefaultShininess;			// デフォルトの光沢度
	static const float kDefaultReflectivity;		// デフォルトの反射率
#pragma endregion
};