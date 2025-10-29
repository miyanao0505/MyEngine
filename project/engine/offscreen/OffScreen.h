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
	// 初期化
	void Initialize();
	// ルートシグネチャの作成
	void CreateRootSignature();
	// グラフィックスパイプラインの生成
	void CreateGraphicsPipeline();
	// 描画
	void Draw();

public:	// getter
	DirectXBase* GetDxBase() const { return dxBase_; }

private:	// メンバ変数
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	// DirectXBase
	DirectXBase* dxBase_ = nullptr;
};

