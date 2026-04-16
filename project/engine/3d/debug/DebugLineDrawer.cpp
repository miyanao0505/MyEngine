#include "DebugLineDrawer.h"
#include "CameraManager.h"

using namespace MyBase;

/// 初期化
void DebugLineDrawer::Initialize(DebugLineBase* base, uint32_t maxLineCount) {
	base_ = base;
	dxBase_ = base->GetDxBase();
	maxVertexCount_ = maxLineCount * kVertexNum; // 1ライン = 2頂点
	vertices_.reserve(maxVertexCount_);
	CreateVertexBuffer();
	vertexBuffer_->Map(0, nullptr, &mappedData_);
}

/// 全ラインの描画
void DebugLineDrawer::Draw(const std::vector<DebugLine>& lines) {
	if (lines.empty()) return;

	base_->UpdateMatrix(CameraManager::GetInstance()->GetCamera()->GetViewProjectionMatrix());

	UpdateVertexBuffer(lines);

	base_->SetCommonScreen();

	auto cmd = dxBase_->GetCommandList();

	cmd->IASetVertexBuffers(0, 1, &vbView_);
	cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	cmd->SetGraphicsRootConstantBufferView(0, base_->GetTransformCB());
	cmd->SetGraphicsRootConstantBufferView(1, base_->GetMaterialCB());

	// vertices_ は「1ライン = 2頂点」なので size() をそのまま使用
	cmd->DrawInstanced(UINT(vertices_.size()), 1, 0, 0);
}

/// VertexBuffer生成
void DebugLineDrawer::CreateVertexBuffer() {
	const UINT size = sizeof(DebugLineVertexData) * maxVertexCount_;
	vertexBuffer_ = dxBase_->CreateBufferResource(size);
	vbView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vbView_.StrideInBytes = sizeof(DebugLineVertexData);
	vbView_.SizeInBytes = size;
}

/// VertexBuffer更新
void DebugLineDrawer::UpdateVertexBuffer(const std::vector<DebugLine>& lines) {
	vertices_.clear();
	for (const auto& line : lines) {
		Vector4 color = line.isHit ? kHitColor : line.color; // 当たっているラインは赤色で描画

		vertices_.push_back({ line.start, color });
		vertices_.push_back({ line.end, color });
	}
	memcpy(mappedData_, vertices_.data(), sizeof(DebugLineVertexData) * vertices_.size());
}