#pragma once
#include <d3d12.h>
#include <wrl.h>

class DirectXBase;

// スプライト共通部
class OffScreen
{

public:	// メンバ関数
	// 初期化
	void Initialize(DirectXBase* dxBase);
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

