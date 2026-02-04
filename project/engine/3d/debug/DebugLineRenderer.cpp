#include "DebugLineRenderer.h"
#include <cassert>

using namespace MyBase;

/// 初期化
void DebugLineRenderer::Initialize(DebugLineBase* base) {
	assert(base);
	base_ = base;
}

/// ライン追加
void DebugLineRenderer::AddLine(const DebugLine& line) {
	lines_.push_back(line);
}

/// ライン全消去
void DebugLineRenderer::Clear() {
	lines_.clear();
}

/// 全ラインの描画
void DebugLineRenderer::DrawAll() {
	if (lines_.empty()) return;

	auto* cmdList = base_->GetDxBase()->GetCommandList();

	// 共通描画設定
	base_->SetCommonScreen();

	// 頂点更新
	UpdateVertexBuffer();

	// VertexBuffer設定
	cmdList->IASetVertexBuffers(0, 1, &vbView_);

	// LineList描画
	cmdList->DrawInstanced(static_cast<UINT>(vertices_.size()), 1, 0, 0);
}

void DebugLineRenderer::UpdateVertexBuffer() {
	vertices_.clear();
	vertices_.reserve(lines_.size() * 2);

	// CPU側で頂点生成
	for (const auto& line : lines_) {
		Vector4 color = line.color;

		// 当たり判定用色切り替え(hit時->赤)
		if (line.isHit) color = Vector4{ 1.0f, 0.0f, 0.0f, 1.0f };

		vertices_.push_back({ line.start, color });
		vertices_.push_back({ line.end, color });
	}

	const size_t vertexCount = vertices_.size();
	if (vertexCount == 0) return;

	const UINT bufferSize = static_cast<UINT>(sizeof(DebugLineVertexData) * vertexCount);

	auto* dxBase = base_->GetDxBase();
	auto* device = dxBase->GetDevice();

	// 頂点数が増えた場合のみ再確保
	if (!vertexBuffer_ || currentVertexCapacity_ < vertexCount) {
		currentVertexCapacity_ = vertexCount;

		D3D12_HEAP_PROPERTIES heapProps{};
		heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

		D3D12_RESOURCE_DESC resourcesDesc{};
		resourcesDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourcesDesc.Width = bufferSize;
		resourcesDesc.Height = 1;
		resourcesDesc.DepthOrArraySize = 1;
		resourcesDesc.MipLevels = 1;
		resourcesDesc.SampleDesc.Count = 1;
		resourcesDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		HRESULT hr = device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourcesDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertexBuffer_)
		);
		assert(SUCCEEDED(hr));

		vbView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
		vbView_.StrideInBytes = sizeof(DebugLineVertexData);
		vbView_.SizeInBytes = bufferSize;
	}

	// 転送
	void* mapped = nullptr;
	vertexBuffer_->Map(0, nullptr, &mapped);
	memcpy(mapped, vertices_.data(), bufferSize);
	vertexBuffer_->Unmap(0, nullptr);
}