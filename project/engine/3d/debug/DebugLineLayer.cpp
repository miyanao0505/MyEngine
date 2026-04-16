#include "DebugLineLayer.h"

/// 初期化
void DebugLineLayer::Initialize(uint32_t maxLineCount) {
	maxLineCount_ = maxLineCount;
	lines_.reserve(maxLineCount_);
}

/// フレーム開始時に呼ぶ
void DebugLineLayer::BeginFrame() {
	lines_.clear();
}

/// ラインの追加
void DebugLineLayer::AddLine(const MyBase::DebugLine& line) {
	if(lines_.size() >= maxLineCount_) {
		return;
	}
	lines_.push_back(line);
}