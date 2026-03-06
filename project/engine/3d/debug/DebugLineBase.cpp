#include "DebugLineBase.h"
#include <cassert>
#include "MyBase.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

using namespace Microsoft::WRL;
using namespace std;
using namespace MyBase;

#ifdef _DEBUG
/// static member 定義
unique_ptr<DebugLineBase> DebugLineBase::sInstance_ = nullptr;

/// Singleton Instance を取得
DebugLineBase* DebugLineBase::GetInstance() {
	if (!sInstance_) {
		sInstance_ = make_unique<DebugLineBase>(DebugLineBase::ConstructorKey{});
	}
	return sInstance_.get();
}

/// 終了処理
void DebugLineBase::Finalize() {
	sInstance_.reset();
}

void DebugLineBase::UpdateMatrix(const MyBase::Matrix4x4& wvp) {
	DebugLineTransformationMatrix* mapped = nullptr;
	transformCB_->Map(0, nullptr, reinterpret_cast<void**>(&mapped));
	mapped->WVP = wvp;
	transformCB_->Unmap(0, nullptr);
}

void DebugLineBase::UpdateMaterial(const MyBase::Vector4& color) {
	DebugLineMaterial* mapped = nullptr;
	materialCB_->Map(0, nullptr, reinterpret_cast<void**>(&mapped));
	mapped->color = color;
	materialCB_->Unmap(0, nullptr);
}

/// 初期化
void DebugLineBase::Initialize(DirectXBase* dxBase) {
	dxBase_ = dxBase;

	CreateGraphicsPipeline();

	transformCB_ = dxBase_->CreateBufferResource(sizeof(DebugLineTransformationMatrix));
	materialCB_ = dxBase_->CreateBufferResource(sizeof(DebugLineMaterial));
}

// 共通画面設定
void DebugLineBase::SetCommonScreen() {
	auto* cmd = dxBase_->GetCommandList();
	cmd->SetGraphicsRootSignature(rootSignature_.Get());
	cmd->SetPipelineState(graphicsPipelineState_.Get());

	

	cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

/// デバッグ更新
void DebugLineBase::DebugUpdate() {
	// 深度バッファの有効/無効設定
	ImGui::Checkbox("Depth Enabled", &depthEnabled_);
	ImGui::Text("\n");
}

/// デバッグ描画
void DebugLineBase::DebugDraw() {
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("DebugLine")) {
		DebugUpdate();
	}
	ImGui::PopID();
}

/// ルートシグネチャを作成
void DebugLineBase::CreateRootSignature() {
	HRESULT hr;

	// CBV : b0 (Transform), b1 (Material)
	D3D12_ROOT_PARAMETER params[2] = {};

	// b0 : TransformationMatrix
	params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	params[0].Descriptor.ShaderRegister = 0;

	// b1 : DebugLineMaterial
	params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	params[1].Descriptor.ShaderRegister = 1;

	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	desc.NumParameters = _countof(params);
	desc.pParameters = params;
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
	depth.DepthEnable = depthEnabled_ ? true : false;
	depth.StencilEnable = false;
	if (depthEnabled_) {
		depth.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depth.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	} else {
		depth.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		depth.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		desc.DSVFormat = DXGI_FORMAT_UNKNOWN;
	}
	desc.DepthStencilState = depth;

	hr = dxBase_->GetDevice()->CreateGraphicsPipelineState(
		&desc,
		IID_PPV_ARGS(&graphicsPipelineState_)
	);
	assert(SUCCEEDED(hr));
}

/// 深度バッファの有効/無効設定
void DebugLineBase::SetDepthEnabled(bool enabled) {
	depthEnabled_ = enabled;
	// 既に DirectXBase がセットされているなら PSO を再作成
	if (dxBase_) {
		CreateGraphicsPipeline();
	}
}

void DebugLineBase::SetBlendMode(DebugLineBlendMode blendMode) {
	blendMode_ = blendMode;

	// ※ 頻繁に呼ぶ想定ではない(デバッグ用途)
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
#endif // _DEBUG