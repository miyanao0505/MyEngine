#pragma once
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include "DebugLineBase.h"
#include "MyBase.h"

#ifdef _DEBUG
/// <summary>
/// デバッグ3Dライン描画管理クラス
/// </summary>
class DebugLineRenderer
{
private:
#pragma region 定数
	static constexpr uint32_t kMaxLineCount = 2048;
#pragma endregion

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
	/// フレーム開始時に呼ぶ
	/// </summary>
	void BeginFrame();

private:	// メンバ関数
	/// <summary>
	/// VertexBuffer生成
	/// </summary>
	void CreateVertexBuffer();

	/// <summary>
	/// VertexBuffer更新
	/// </summary>
	void UpdateVertexBuffer();

private:	// メンバ変数
	// DebugLineBase
	DebugLineBase* base_ = nullptr;
	// DirectXBase
	DirectXBase* dxBase_ = nullptr;

	// ライン情報
	std::vector<MyBase::DebugLine> lines_;

	// GPU用頂点データ
	std::vector<MyBase::DebugLineVertexData> vertices_;

	// VertexBuffer
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
};
#endif // _DEBUG