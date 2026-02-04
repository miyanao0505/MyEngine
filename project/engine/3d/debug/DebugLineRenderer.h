#pragma once
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include "DebugLineBase.h"
#include "MyBase.h"

/// <summary>
/// デバッグ3Dライン描画管理クラス
/// </summary>
class DebugLineRenderer
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="base">DebugLineBase</param>
	void Initialize(DebugLineBase* base);

	/// <summary>
	/// ライン追加
	/// </summary>
	/// <param name="line">ライン情報</param>
	void AddLine(const MyBase::DebugLine& line);
	
	/// <summary>
	/// 全ラインの描画
	/// </summary>
	void DrawAll();
	
	/// <summary>
	/// ライン全消去(毎フレーム呼ぶ想定)
	/// </summary>
	void Clear();

private:	// メンバ関数
	/// <summary>
	/// VertexBuffer生成・更新
	/// </summary>
	void UpdateVertexBuffer();

private:	// メンバ変数
	// DebugLineBase
	DebugLineBase* base_ = nullptr;

	// ライン情報
	std::vector<MyBase::DebugLine> lines_;

	// GPU用頂点データ
	std::vector<MyBase::DebugLineVertexData> vertices_;

	// VertexBuffer
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	// 現在の確保頂点数
	size_t currentVertexCapacity_ = 0;
};