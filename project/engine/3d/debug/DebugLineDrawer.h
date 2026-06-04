#pragma once
#include <vector>
#include "DebugLineBase.h"

#ifdef _DEBUG
/// <summary>
/// デバッグライン描画クラス
/// DebugLineManager から呼び出され、実際の描画処理を行うクラス。
/// </summary>
class DebugLineDrawer {
private:
#pragma region 定数
	static constexpr uint32_t kVertexNum = 2;

	static constexpr MyBase::Vector4 kHitColor = { 1.0f, 0.0f, 0.0f, 1.0f };	// ヒットしているラインの色
#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="base">デバッグラインの基底クラス</param>
	/// <param name="maxLineCount">最大ライン数</param>
	void Initialize(DebugLineBase* base, uint32_t maxLineCount);

	/// <summary>
	/// 全ラインの描画
	/// </summary>
	/// <param name="lines">描画するラインの配列</param>
	void Draw(const std::vector<MyBase::DebugLine>& lines);

private:	// メンバ関数
	/// <summary>
	/// VertexBuffer生成
	/// </summary>
	void CreateVertexBuffer();
	/// <summary>
	/// VertexBuffer更新
	/// </summary>
	void UpdateVertexBuffer(const std::vector<MyBase::DebugLine>& lines);

private:	// メンバ変数
	// DebugLineBase
	DebugLineBase* base_ = nullptr;
	// DirectXBase
	DirectXBase* dxBase_ = nullptr;
	// ライン情報
	std::vector<MyBase::DebugLineVertexData> vertices_;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	/// マップ用ポインタ
	void* mappedData_ = nullptr;

	uint32_t maxVertexCount_ = 0;
};
#endif // _DEBUG