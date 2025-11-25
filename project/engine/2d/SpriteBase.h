#pragma once
#include "DirectXBase.h"

/// <summary>
/// スプライト描画の共通処理を管理するクラス。
/// 各スプライト(Sprite クラス)は本クラスを通して
/// 描画パイプライン、ルートシグネチャ、ブレンドモード設定を共有する。
/// </summary>
class SpriteBase
{
public:	// 列挙型
	// ブレンドモード
	enum class BlendMode {
		kBlendModeNone,			//!< ブレンドなし
		kBlendModeNormal,		//!< 通常αブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
		kBlendModeAdd,			//!< 加算。 Src * SrcA + Dest * 1
		kBlendModeSubtract,		//!< 減算。 Dest * 1 - Src * SrcA
		kBlendModeMultiply,		//!< 乗算。 Src * 0 + Dest * Src
		kBlendModeScreen,		//!< スクリーン。 Src * (1 - Dest) + Dest * 1
		kBlendModeExclusion,	//!< 除外。(1 - Dest) * Src + (1 - Src) * Dest

		kCountOfBlendMode,	//!< 利用してはいけない
	};

public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxBase">DirectX の基本機能を提供する DirectXBase へのポインタ</param>
	void Initialize(DirectXBase* dxBase);

	/// <summary>
	/// 共通の画面設定(描画ターゲット、ビューポートなど)を適用
	/// </summary>
	void SetCommonScreen();

	/// <summary>
	/// スプライト描画用のグラフィックスパイプラインステートを生成
	/// </summary>
	void CreateGraphicsPipeline();

public:	// getter
	/// <summary>
	/// DirectXBase のポインタを取得
	/// </summary>
	/// <returns>DirectXBase へのポインタ</returns>
	DirectXBase* GetDxBase() const { return dxBase_; }

public:	// setter
	/// <summary>
	/// 現在のブレンドモードを設定
	/// </summary>
	/// <param name="blendMode">適用するブレンドモード。BlendMode 列挙型を使用します</param>
	void SetBlendMode(BlendMode blendMode);

private:	// メンバ関数
	/// <summary>
	/// スプライト描画用のルートシグネチャを作成
	/// </summary>
	void CreateRootSignature();

	// ブレンドモード設計
	D3D12_BLEND_DESC SetBlendModeNone();
	D3D12_BLEND_DESC SetBlendModeNormal();
	D3D12_BLEND_DESC SetBlendModeAdd();
	D3D12_BLEND_DESC SetBlendModeSubtract();
	D3D12_BLEND_DESC SetBlendModeMultiply();
	D3D12_BLEND_DESC SetBlendModeScreen();
	static D3D12_BLEND_DESC(SpriteBase::* spFuncTable[])();

private:	// メンバ変数
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	// ブレンドモード
	BlendMode blendMode_ = BlendMode::kBlendModeNormal;

	// DirectXBase
	DirectXBase* dxBase_ = nullptr;
};

