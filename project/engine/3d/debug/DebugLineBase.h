#pragma once
#include "DirectXBase.h"

/// <summary>
/// デバッグライン描画ブレンドモード
/// </summary>
enum class DebugLineBlendMode {
	kNone,
	kAlppha,
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
	/// 初期化
	/// </summary>
	/// <param name="dxBase">DirectXBase</param>
	void Initilize(DirectXBase* dxBase);

	/// <summary>
	/// 共通描画設定
	/// </summary>
	void SetCommonScreen();

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

public:	// setter
	/// <summary>
	/// ブレンドモード設定
	/// </summary>
	/// <param name="blendMode">ブレンドモード</param>
	void SetBlendMode(DebugLineBlendMode blendMode);

	// --- BlendMode ---
	D3D12_BLEND_DESC SetBlendNone();
	D3D12_BLEND_DESC SetBlendAlpha();
	D3D12_BLEND_DESC SetBlendAdd();

private:	// メンバ変数
	// DirectXBase
	DirectXBase* dxBase_ = nullptr;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;

	// Blend
	DebugLineBlendMode blendMode_ = DebugLineBlendMode::kAlppha;
	static D3D12_BLEND_DESC(DebugLineBase::* spBlendTable[])();
};

