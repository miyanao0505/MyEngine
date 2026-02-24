#include "DebugLineRenderer.h"
#include <cassert>
#include "CameraManager.h"

using namespace MyBase;

/// 初期化
void DebugLineRenderer::Initialize(DebugLineBase* base) {
	assert(base);
	base_ = base;
	dxBase_ = base->GetDxBase();
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
#ifdef _DEBUG
	if(lines_.empty()) return;
	
	UpdateVertexBuffer();

	base_->SetCommonScreen();

	auto cmd = dxBase_->GetCommandList();
	cmd->IASetVertexBuffers(0, 1, &vbView_);

	cmd->DrawInstanced(UINT(vertices_.size()), 1, 0, 0);
#endif // _DEBUG
}

/// VertexBuffer生成
void DebugLineRenderer::CreateVertexBuffer() {
	const UINT size = UINT(sizeof(DebugLineVertexData) * vertices_.size());

	vertexBuffer_ = dxBase_->CreateBufferResource(size);
	vbView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vbView_.StrideInBytes = sizeof(DebugLineVertexData);
	vbView_.SizeInBytes = size;
}

/// VertexBuffer更新
void DebugLineRenderer::UpdateVertexBuffer() {
	vertices_.clear();
	
	for (auto& line : lines_) {
		Vector4 color = line.isHit ? Vector4{ 1.0f, 0.0f, 0.0f, 1.0f } : line.color;

		vertices_.push_back({ line.start, color });
		vertices_.push_back({ line.end, color });
	}

	CreateVertexBuffer();

	void* data = nullptr;
	vertexBuffer_->Map(0, nullptr, &data);
	memcpy(data, vertices_.data(), sizeof(DebugLineVertexData) * vertices_.size());
	vertexBuffer_->Unmap(0, nullptr);
}