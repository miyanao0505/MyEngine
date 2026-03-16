#pragma once
#include "DirectXBase.h"

#ifdef _DEBUG
/// <summary>
/// デバッグライン描画ブレンドモード
/// </summary>
enum class DebugLineBlendMode {
	kNone,
	kAlpha,
	kAdd,

	kCount,	// 使用禁止
};

/// <summary>
/// デバッグライン描画共通基底クラス
/// PSO / RootSignature / 共通描画設定のみを管理する
/// </summary>
class DebugLineBase
{
public:	// メンバ関数
	/// <summary>
	/// Singleton Instance を取得
	/// </summary>
	/// <returns>DebugLineBase</returns>
	static DebugLineBase* GetInstance();

	/// ------ Passkey Idion ------
	/// コントラクタを渡すための鍵
	class ConstructorKey {
	private:
		ConstructorKey() = default;
		friend class DebugLineBase;
	};

	/// PassKeyを受け取るコンストラクタ
	explicit DebugLineBase(ConstructorKey) {}

	/// コピー禁止
	DebugLineBase(const DebugLineBase&) = delete;
	DebugLineBase& operator=(const DebugLineBase&) = delete;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxBase">DirectXBase</param>
	void Initialize(DirectXBase* dxBase);

	/// <summary>
	/// 共通描画設定
	/// </summary>
	void SetCommonScreen();

	/// <summary>
	/// 終了処理(unique_ptr をリセット)
	/// </summary>
	static void Finalize();

	/// <summary>
	/// 行列の更新
	/// </summary>
	/// <param name="wvp">wvp</param>
	void UpdateMatrix(const MyBase::Matrix4x4& wvp);
	
	/// <summary>
	/// マテリアルの更新
	/// </summary>
	/// <param name="color">色</param>
	void UpdateMaterial(const MyBase::Vector4& color);

	/// <summary>
	/// デバッグ更新
	/// </summary>
	void DebugUpdate();

	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw();

private:
	/// <summary>
	/// デバッグ3Dライン描画用のルートシグネチャを作成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// デバッグ3Dライン描画用のグラフィックスパイプラインステートを生成
	/// </summary>
	void CreateGraphicsPipeline();

public:	// getter
	/// <summary>
	/// DirectXBaseの取得
	/// </summary>
	/// <returns>DirectXBase</returns>
	DirectXBase* GetDxBase() const { return dxBase_; }

	/// <summary>
	/// グラフィックパイプラインステートの取得
	/// </summary>
	/// <returns>GraphicsPipelineState</returns>
	ID3D12PipelineState* GetGraphicsPipelineState() const { return graphicsPipelineState_.Get(); }

	/// <summary>
	/// ルートシグネチャの取得
	/// </summary>
	/// <returns>RootSignature</returns>
	ID3D12RootSignature* GetRootSignature() const { return rootSignature_.Get(); }

	D3D12_GPU_VIRTUAL_ADDRESS GetTransformCB() const { return transformCB_.Get()->GetGPUVirtualAddress(); };
	D3D12_GPU_VIRTUAL_ADDRESS GetMaterialCB() const { return materialCB_.Get()->GetGPUVirtualAddress(); };

public:	// setter
	/// <summary>
	/// 深度バッファの有効/無効設定
	/// </summary>
	/// <param name="enabled">深度バッファ有効フラグ</param>
	void SetDepthEnabled(bool enabled);

	/// <summary>
	/// ブレンドモード設定
	/// </summary>
	/// <param name="blendMode">ブレンドモード</param>
	void SetBlendMode(DebugLineBlendMode blendMode);

	// --- BlendMode ---
	D3D12_BLEND_DESC SetBlendNone();
	D3D12_BLEND_DESC SetBlendAlpha();
	D3D12_BLEND_DESC SetBlendAdd();

private:	// Singleton Instance
	static std::unique_ptr<DebugLineBase> sInstance_;

private:	// メンバ変数
	// DirectXBase
	DirectXBase* dxBase_ = nullptr;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;

	Microsoft::WRL::ComPtr<ID3D12Resource> transformCB_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialCB_;

	// 深度バッファ有効フラグ
	bool depthEnabled_ = true;

	// Blend
	DebugLineBlendMode blendMode_ = DebugLineBlendMode::kAlpha;
	static D3D12_BLEND_DESC(DebugLineBase::* spBlendTable[])();
};
#endif // _DEBUG