#pragma once
#include <d3d12.h>
#include <wrl.h>

// 前方宣言
class DirectXBase;

/// <summary>
/// スプライト描画共通基底クラス(OffScreen)
/// </summary>
class OffScreen
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ルートシグネチャの作成
	/// </summary>
	void CreateRootSignature();
	
	/// <summary>
	/// グラフィックスパイプラインの生成
	/// </summary>
	void CreateGraphicsPipeline();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public:	// getter
	/// <summary>
	/// DirectXBaseの取得
	/// </summary>
	/// <returns>DirectXBaseのインスタンス</returns>
	DirectXBase* GetDxBase() const { return dxBase_; }

private:	// メンバ変数
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	// DirectXBase
	DirectXBase* dxBase_ = nullptr;
};