#pragma once
#include "DirectXBase.h"

/// <summary>
/// パーティクル・スプライト描画用ブレンドモード列挙型(BlendMode)
/// </summary>
enum class BlendMode {
	kBlendModeNone,			//!< ブレンドなし
	kBlendModeNormal,		//!< 通常αブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
	kBlendModeAdd,			//!< 加算。 Src * SrcA + Dest * 1
	kBlendModeSubtract,		//!< 減算。 Dest * 1 - Src * SrcA
	kBlendModeMultiply,		//!< 乗算。 Src * 0 + Dest * Src
	kBlendModeScreen,		//!< スクリーン。 Src * (1 - Dest) + Dest * 1

	kCountOfBlendMode,	//!< 利用してはいけない
};

/// <summary>
/// パーティクル描画共通基底クラス(ParticleBase)
/// </summary>
class ParticleBase
{
public:	// 列挙型
	
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxBase">DirectXBaseクラスのインスタンス</param>
	void Initialize(DirectXBase* dxBase);

	/// <summary>
	/// ルートシグネチャの作成
	/// </summary>
	void CreateRootSignature();
	
	/// <summary>
	/// グラフィックスパイプラインの生成
	/// </summary>
	void CreateGraphicsPipeline();
	
	/// <summary>
	/// 共通画面設定
	/// </summary>
	void SetCommonScreen();

public:	// getter
	/// <summary>
	/// DirectXBaseの取得
	/// </summary>
	/// <returns>DirectXBaseクラスのインスタンス</returns>
	DirectXBase* GetDxBase() const { return dxBase_; }
	
	/// <summary>
	/// ブレンドモードの取得
	/// </summary>
	/// <returns>ブレンドモード</returns>
	BlendMode GetBlendMode() const { return blendMode_; }

public:	// setter
	/// <summary>
	/// ブレンドモードの設定
	/// </summary>
	/// <param name="blendMode">ブレンドモード</param>
	void SetBlendMode(BlendMode blendMode);

private:	// メンバ関数
	// ブレンドモード設計

	/// <summary>
	/// ブレンドなしのブレンド設定
	/// </summary>
	/// <returns>NONE 用 D3D12_BLEND_DESC</returns>
	D3D12_BLEND_DESC SetBlendModeNone();

	/// <summary>
	/// 通常 α ブレンドの設定
	/// </summary>
	/// <returns>NORMAL 用 D3D12_BLEND_DESC</returns>
	D3D12_BLEND_DESC SetBlendModeNormal();
	
	/// <summary>
	/// 加算ブレンドの設定
	/// </summary>
	/// <returns>ADD 用 D3D12_BLEND_DESC</returns>
	D3D12_BLEND_DESC SetBlendModeAdd();
	
	/// <summary>
	/// 減算ブレンドの設定
	/// </summary>
	/// <returns>SUBTRACT 用 D3D12_BLEND_DESC</returns>
	D3D12_BLEND_DESC SetBlendModeSubtract();
	
	/// <summary>
	/// 乗算ブレンドの設定
	/// </summary>
	/// <returns>MULTIPLY 用 D3D12_BLEND_DESC</returns>
	D3D12_BLEND_DESC SetBlendModeMultiply();
	
	/// <summary>
	/// スクリーンブレンドの設定を返す。
	/// </summary>
	/// <returns>SCREEN 用 D3D12_BLEND_DESC</returns>
	D3D12_BLEND_DESC SetBlendModeScreen();

private:	// メンバ変数
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	// ブレンドモード
	BlendMode blendMode_ = BlendMode::kBlendModeAdd;
	static D3D12_BLEND_DESC(ParticleBase::* spFuncTable[])();

	// DirectXBase
	DirectXBase* dxBase_;
};