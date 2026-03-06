#include "DebugLineRenderer.h"
#include <cassert>
#include "CameraManager.h"

using namespace MyBase;

#ifdef _DEBUG
/// 初期化
void DebugLineRenderer::Initialize(DebugLineBase* base) {
	assert(base);
	base_ = base;
	dxBase_ = base->GetDxBase();

	vertices_.reserve(kMaxLineCount * 2);
	lines_.reserve(kMaxLineCount);
	CreateVertexBuffer();
}

/// ライン追加
void DebugLineRenderer::AddLine(const DebugLine& line) {
	if(lines_.size() >= kMaxLineCount) {
		return;
	}
	lines_.push_back(line);
}

/// フレーム開始時に呼ぶ
void DebugLineRenderer::BeginFrame() {
	lines_.clear();
}

/// 全ラインの描画
void DebugLineRenderer::DrawAll() {
	if(lines_.empty()) return;
	
	UpdateVertexBuffer();

	base_->SetCommonScreen();

	auto cmd = dxBase_->GetCommandList();
	cmd->IASetVertexBuffers(0, 1, &vbView_);
	cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	
	cmd->SetGraphicsRootConstantBufferView(0, base_->GetTransformCB());
	cmd->SetGraphicsRootConstantBufferView(1, base_->GetMaterialCB());

	// vertices_ は「1ライン = 2頂点」なので size() をそのまま使用
	cmd->DrawInstanced(UINT(vertices_.size()), 1, 0, 0);

	// 描画後はライン情報をクリア
	BeginFrame();
}

/// VertexBuffer生成
void DebugLineRenderer::CreateVertexBuffer() {
	const UINT size = UINT(sizeof(DebugLineVertexData) * kMaxLineCount * 2);

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

	void* data = nullptr;
	vertexBuffer_->Map(0, nullptr, &data);
	memcpy(data, vertices_.data(), sizeof(DebugLineVertexData) * vertices_.size());
	vertexBuffer_->Unmap(0, nullptr);
}
#endif // _DEBUG