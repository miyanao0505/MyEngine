#include "DebugLineBase.h"
#include <cassert>

using namespace Microsoft::WRL;
using namespace std;

/// static member 定義
unique_ptr<DebugLineBase> DebugLineBase::sInstance_ = nullptr;

/// Singleton Instance を取得
DebugLineBase* DebugLineBase::GetInstance() {
	if (sInstance_ == nullptr) {
		sInstance_ = make_unique<DebugLineBase>(ConstructorKey());
	}
	return sInstance_.get();
}

/// 終了処理
void DebugLineBase::Finalize() {
	sInstance_.reset();
}

/// 初期化
void DebugLineBase::Initilize(DirectXBase* dxBase) {
	dxBase_ = dxBase;

	CreateGraphicsPipeline();
}

// 共通画面設定
void DebugLineBase::SetCommonScreen() {
	auto* cmd = dxBase_->GetCommandList();
	cmd->SetGraphicsRootSignature(rootSignature_.Get());
	cmd->SetPipelineState(graphicsPipelineState_.Get());
	cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

/// ルートシグネチャを作成
void DebugLineBase::CreateRootSignature() {
	HRESULT hr;

	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// DebugLine は SRV / CBV 不要
	desc.NumParameters = 0;
	desc.pParameters = nullptr;
	desc.NumStaticSamplers = 0;
	desc.pStaticSamplers = nullptr;

	ComPtr<ID3DBlob> blob;
	ComPtr<ID3DBlob> error;

	hr = D3D12SerializeRootSignature(
		&desc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&blob, &error
	);
	if (FAILED(hr)) {
		assert(false);
	}

	hr = dxBase_->GetDevice()->CreateRootSignature(
		0,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_)
	);
	assert(SUCCEEDED(hr));
}

/// グラフィックスパイプラインステートを生成
void DebugLineBase::CreateGraphicsPipeline() {
	HRESULT hr;
	CreateRootSignature();

	// InputLayout : POSITION + COLOR
	D3D12_INPUT_ELEMENT_DESC inputElements[2] = {};
	inputElements[0] = {
		"POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
	};
	inputElements[1] = {
		"COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
	};
	D3D12_INPUT_LAYOUT_DESC inputLayout{};
	inputLayout.pInputElementDescs = inputElements;
	inputLayout.NumElements = _countof(inputElements);

	// Shader
	auto vs = dxBase_->CompileShader(L"resources/shaders/DebugLine.VS.hlsl", L"vs_6_0");
	auto ps = dxBase_->CompileShader(L"resources/shaders/DebugLine.PS.hlsl", L"ps_6_0");

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
	desc.pRootSignature = rootSignature_.Get();
	desc.InputLayout = inputLayout;
	desc.VS = { vs->GetBufferPointer(), vs->GetBufferSize() };
	desc.PS = { ps->GetBufferPointer(), ps->GetBufferSize() };

	// Blend
	desc.BlendState = (this->*spBlendTable[static_cast<size_t>(blendMode_)])();

	// Rasterizer
	D3D12_RASTERIZER_DESC raster{};
	raster.FillMode = D3D12_FILL_MODE_SOLID;
	raster.CullMode = D3D12_CULL_MODE_NONE;
	desc.RasterizerState = raster;

	// Line
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

	// RenderTarget
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.SampleDesc.Count = 1;
	desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// Depth
	D3D12_DEPTH_STENCIL_DESC depth{};
	depth.DepthEnable = true;
	depth.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depth.DepthFunc = D3D12_COMPARISON_FUNC_EQUAL;
	desc.DepthStencilState = depth;
	desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	hr = dxBase_->GetDevice()->CreateGraphicsPipelineState(
		&desc,
		IID_PPV_ARGS(&graphicsPipelineState_)
	);
	assert(SUCCEEDED(hr));
}

void DebugLineBase::SetBlendMode(DebugLineBlendMode blendMode) {
	blendMode_ = blendMode;
	CreateGraphicsPipeline();
}

/// --- BlendMode ---
D3D12_BLEND_DESC DebugLineBase::SetBlendNone() {
	D3D12_BLEND_DESC desc{};
	desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	return desc;
}

D3D12_BLEND_DESC DebugLineBase::SetBlendAlpha() {
	D3D12_BLEND_DESC desc{};
	desc.RenderTarget[0].BlendEnable = TRUE;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	desc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;

	desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;

	return desc;
}

D3D12_BLEND_DESC DebugLineBase::SetBlendAdd() {
	D3D12_BLEND_DESC desc{};
	desc.RenderTarget[0].BlendEnable = TRUE;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	desc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;

	desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;

	return desc;
}

D3D12_BLEND_DESC(DebugLineBase::* DebugLineBase::spBlendTable[])() = {
	&DebugLineBase::SetBlendNone,
	&DebugLineBase::SetBlendAlpha,
	&DebugLineBase::SetBlendAdd,
};