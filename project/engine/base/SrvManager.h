#pragma once
#include "DirectXBase.h"

/// <summary>
/// SRV(Shader Resource View)を一元管理するクラス
/// </summary>
class SrvManager
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// Allocationの確保
	/// </summary>
	/// <returns>確保したSRVインデックス</returns>
	uint32_t Allocate();

	/// <summary>
	/// SRV生成(テクスチャ用)
	/// </summary>
	/// <param name="srvIndex">SRVインデックス</param>
	/// <param name="metaData">テクスチャメタデータ</param>
	/// <param name="pResource">テクスチャリソース</param>
	void CreateSRVForTexture2D(uint32_t srvIndex, DirectX::TexMetadata metaData, ID3D12Resource* pResource);
	
	/// <summary>
	/// SRV生成(Structured Buffer用)
	/// </summary>
	/// <param name="srvIndex">SRVインデックス</param>
	/// <param name="pResource">バッファリソース</param>
	/// <param name="numElements">要素数</param>
	/// <param name="structureByteStride">構造体のバイトサイズ</param>
	void CreateSRVForStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);
	
	/// <summary>
	/// SRV生成(キューブマップ用)
	/// </summary>
	/// <param name="srvIndex">SRVインデックス</param>
	/// <param name="metaData">テクスチャメタデータ</param>
	/// <param name="pResource">テクスチャリソース</param>
	void CreateSRVForTextureCube(uint32_t srvIndex, DirectX::TexMetadata metaData, ID3D12Resource* pResource);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// SRVセットコマンド
	/// </summary>
	/// <param name="RootParameterIndex">ルートパラメータインデックス</param>
	/// <param name="srvIndex">SRVインデックス</param>
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

	/// <summary>
	/// テクスチャ枚数上限チェック
	/// </summary>
	/// <returns>安全ならtrueを返す</returns>
	bool IsSecure();

public:	// getter
	/// <summary>
	/// SRVの指定番号のCPUデスクリプタハンドルを取得する
	/// </summary>
	/// <param name="index">SRVインデックス</param>
	/// <returns>CPUデスクリプタハンドル</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	
	/// <summary>
	/// SRVの指定番号のGPUデスクリプタハンドルを取得する
	/// </summary>
	/// <param name="index">SRVインデックス</param>
	/// <returns>GPUデスクリプタハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);
	
	/// <summary>
	/// ImGui用のデスクリプタヒープを取得
	/// </summary>
	/// <returns>デスクリプタヒープ</returns>
	ID3D12DescriptorHeap* GetDescriptorHeapForImGui() { return descriptorHeap_.Get(); }

private:	// メンバ変数
	DirectXBase* dxBase_ = nullptr;

	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;
	// デスクリプタサイズ
	uint32_t descriptorSize_;

	// 次に使用するSRVインデックス
	uint32_t useIndex_ = 0;

#pragma region 定数
public:
	// 最大SRV数(最大テクスチャ枚数)
	static const uint32_t kMaxSRVCount;

private:
	static constexpr UINT kDescriptorHeapCount = 1;
	static constexpr UINT kFirstElementOffset = 0;
#pragma endregion
};