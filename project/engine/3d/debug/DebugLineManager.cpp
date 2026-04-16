#include "DebugLineManager.h"
#include "DebugLinePrimitive.h"

using namespace std;
using namespace MyBase;

/// static member 定義
unique_ptr<DebugLineManager> DebugLineManager::sInstance_ = nullptr;

/// Singleton Instance を取得
DebugLineManager* DebugLineManager::GetInstance() {
	if (!sInstance_) {
		sInstance_ = make_unique<DebugLineManager>(DebugLineManager::ConstructorKey{});
	}
	return sInstance_.get();
}

/// 初期化
void DebugLineManager::Initialize() {
	// レイヤーの初期化
	for (auto& layer : layers_) {
		layer.Initialize(1000); // 最大1000ライン
	}
	// 描画クラスの初期化
	drawer_ = make_unique<DebugLineDrawer>();
	drawer_->Initialize(DebugLineBase::GetInstance(), 1000);
}

/// 終了
void DebugLineManager::Finalize() {
	drawer_.reset();

	// Singleton Instance の解放
	sInstance_.reset();
}

/// フレーム開始時に呼ぶ
void DebugLineManager::BeginFrame() {
	for (auto& layer : layers_) {
		layer.BeginFrame();
	}
}

/// ラインの追加
void DebugLineManager::AddLine(DebugLineCategory category, const MyBase::DebugLine& line) {
	layers_[size_t(category)].AddLine(line);
}

/// ラインの描画
void DebugLineManager::Draw(DebugLineCategory category) {
	if (!enableFlags_[size_t(category)]) return;	// 無効なカテゴリはスキップ

	drawer_->Draw(layers_[size_t(category)].GetLines());
}

/// 直線の描画
void DebugLineManager::DrawLine(DebugLineCategory category, const MyBase::DebugLine& line) {
	layers_[size_t(category)].AddLine(line);
}

/// 直線の描画
void DebugLineManager::DrawLine(DebugLineCategory category, const MyBase::Vector3& start, const MyBase::Vector3& end, const MyBase::Vector4& color) {
	DrawLine(category, { start, end, color, false });
}

/// スフィアの描画
void DebugLineManager::DrawSphere(DebugLineCategory category, const MyBase::Sphere& sphere, const MyBase::Vector4& color, uint32_t segment, bool isHit) {
	workLines_.clear();

	DebugLinePrimitive::CreateSphere(workLines_, sphere.center, sphere.radius, segment, color, isHit);

	for (auto& l : workLines_) {
		layers_[size_t(category)].AddLine(l);
	}
}

/// スフィアの描画
void DebugLineManager::DrawSphere(DebugLineCategory category, const MyBase::Vector3& center, float radius, const MyBase::Vector4& color, uint32_t segment, bool isHit) {
	DrawSphere(category, Sphere{ center, radius }, color, segment, isHit);
}

/// ボックスの描画
void DebugLineManager::DrawBox(DebugLineCategory category, const MyBase::Vector3& center, const MyBase::Vector3& size, const MyBase::Matrix4x4& worldMatrix, const MyBase::Vector4& color, bool isHit) {
	workLines_.clear();

	DebugLinePrimitive::CreateBox(workLines_, center, size, worldMatrix, color, isHit);

	for (auto& l : workLines_) {
		layers_[size_t(category)].AddLine(l);
	}
}

/// OBBの描画
void DebugLineManager::DrawOBB(DebugLineCategory category, const MyBase::OBB& obb, const MyBase::Vector4& color, bool isHit) {
	workLines_.clear();

	DebugLinePrimitive::CreateOBB(workLines_, obb, color, isHit);

	for (auto& l : workLines_) {
		layers_[size_t(category)].AddLine(l);
	}
}

/// スプラインの描画
void DebugLineManager::DrawSpline(DebugLineCategory category, const std::vector<MyBase::Vector3>& points, uint32_t division, const MyBase::Vector4& color, bool isHit) {
	workLines_.clear();

	DebugLinePrimitive::CreateSpline(workLines_, points, division, color, isHit);

	for (auto& l : workLines_) {
		layers_[size_t(category)].AddLine(l);
	}
}

/// 全てのラインの描画
void DebugLineManager::DrawAll() {
	for (size_t i = 0; i < layers_.size(); ++i) {
		if (!enableFlags_[i]) continue;	// 無効なカテゴリはスキップ

		drawer_->Draw(layers_[i].GetLines());
	}
}