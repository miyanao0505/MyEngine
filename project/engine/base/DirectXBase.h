#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>
#include <array>
#include <chrono>
#include "Logger.h"
#include "StringUtility.h"
#include "WindowsAPI.h"
#include "DirectXTex.h"
#include "MyBase.h"

// 前方宣言
class SrvManager;

/// <summary>
/// DirectX 12 の初期化・描画制御を統括する基盤クラス。
/// </summary>
class DirectXBase
{
public:	// 列挙型
	enum RTVIndex : uint32_t {
		kRTV_BackBuffer0 = 0,
		kRTV_BackBuffer1 = 1,
		kRTV_RenderTexture = 2,
	};

public:	// メンバ関数
	/// <summary>
	/// シングルトンインスタンスを取得
	/// </summary>
	/// <returns>DirectXBase のインスタンス</returns>
	static DirectXBase* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApi">WindowsAPI クラスのインスタンス</param>
	void Initialize(WindowsAPI* winApi);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
	
	/// <summary>
	/// 描画前処理(RenderTexture)
	/// </summary>
	void PreRenderTexture();

	/// <summary>
	/// 描画前処理
	///	</summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// オフスクリーンのSRV作成
	/// </summary>
	/// <param name="srvManager">SrvManagerクラスのインスタンス</param>
	void CreateOffScreenSRV(SrvManager* srvManager);

	/// <summary>
	/// 指定したShaderをCompileをする関数
	/// </summary>
	/// <param name="filePath">CompilerするShaderファイルへのパス</param>
	/// <param name="profile">Compilerに使用するProfile</param>
	/// <returns>生成された Shader バイナリ</returns>
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile);

	/// <summary>
	/// 指定サイズのバッファーリソースの生成
	/// </summary>
	/// <param name="sizeInBytes">バッファサイズ(バイト)</param>
	/// <returns>生成されたバッファリソース</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	/// <summary>
	/// TextureResource作成の関数
	/// </summary>
	/// <param name="metadata">テクスチャのメタデータ</param>
	/// <returns>生成されたテクスチャリソース</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// RenderTextureの生成
	/// </summary>
	/// <param name="width">テクスチャの幅</param>
	/// <param name="height">テクスチャの高さ</param>
	/// <param name="format">テクスチャのフォーマット</param>
	/// <param name="clearColor">クリアカラー</param>
	/// <returns>生成されたRenderTextureリソース</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(uint32_t width, uint32_t height, DXGI_FORMAT format, const MyBase::Vector4& clearColor);

	/// <summary>
	/// テクスチャへ画像データを転送
	/// </summary>
	/// <param name="texture">転送先のテクスチャリソース</param>
	/// <param name="mipImages">転送元画像イメージデータ</param>
	/// <returns>アップロード用バッファリソース</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	/// <summary>
	/// デスクリプタヒープの生成
	/// </summary>
	/// <param name="heapType">デスクリプタヒープのタイプ</param>
	/// <param name="numDescriptors">デスクリプタ数</param>
	/// <param name="shaderVisible">シェーダーから見えるかどうか</param>
	/// <returns>生成されたデスクリプタヒープ</returns>
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	/// <param name="filePath">テクスチャファイルのパス</param>
	/// <returns>画像イメージデータ</returns>
	static DirectX::ScratchImage LoadTexture(const std::string& filePath);

	/// <summary>
	/// リソースに対してバリアを設定し、状態を遷移
	/// </summary>
	/// <param name="pResource">対象リソース</param>
	/// <param name="Before">遷移前の状態</param>
	/// <param name="After">遷移後の状態</param>
	void BarrierTransition(ID3D12Resource* pResource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After);

public:	// getter
	/// <summary>
	/// RTVの指定番号のCPUデスクリプタハンドルを取得
	/// </summary>
	/// <param name="index">デスクリプタ番号</param>
	/// <returns>CPUデスクリプタハンドル</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// RTVの指定番号のGPUデスクリプタハンドルを取得
	/// </summary>
	/// <param name="index">デスクリプタ番号</param>
	/// <returns>GPUデスクリプタハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// DSVの指定番号のCPUデスクリプタハンドルを取得
	/// </summary>
	/// <param name="index">デスクリプタ番号</param>
	/// <returns>CPUデスクリプタハンドル</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// DSVの指定番号のGPUデスクリプタハンドルを取得
	/// </summary>
	/// <param name="index">デスクリプタ番号</param>
	/// <returns>GPUデスクリプタハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// RenderTextureのCPUデスクリプタハンドルを取得
	/// </summary>
	/// <returns>CPUデスクリプタハンドル</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetOffScreenSRVCPUDescriptorHandle() { return offScreenSrvHandleCPU_; };
	
	/// <summary>
	/// RenderTextureのGPUデスクリプタハンドルを取得
	/// </summary>
	/// <returns>GPUデスクリプタハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetOffScreenSRVGPUDescriptorHandle() { return offScreenSrvHandleGPU_; };

	/// <summary>
	/// DirectX12デバイスの取得
	/// </summary>
	/// <returns>DirectX12デバイス</returns>
	ID3D12Device* GetDevice() const { return device_.Get(); }
	
	/// <summary>
	/// コマンドキューの取得
	/// </summary>
	/// <returns>コマンドキュー</returns>
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }
	
	/// <summary>
	/// バックバッファの数を取得
	/// </summary>
	/// <returns>バックバッファの数</returns>
	size_t GetBackBufferCount() const { return backBuffer_.size(); }

private:	// メンバ関数
	/// <summary>
	/// デバイスの生成
	/// </summary>
	void CreateDevice();

	/// <summary>
	/// コマンド関連の生成
	/// </summary>
	void CreateCommand();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// 深度バッファの生成
	/// </summary>
	void CreateDepthStencil();

	/// <summary>
	/// 各種デスクリプタヒープの生成
	/// </summary>
	void CreateDescriptorHeapAllKinds();
	
	/// <summary>
	/// レンダーターゲットビューの初期化
	/// </summary>
	void InitializeRenderTargetView();

	/// <summary>
	/// CPUデスクリプタハンドルを取得
	/// </summary>
	/// <param name="descriptorHeap">デスクリプタヒープ</param>
	/// <param name="descriptorSize">デスクリプタサイズ</param>
	/// <param name="index">デスクリプタ番号</param>
	/// <returns>CPUデスクリプタハンドル</returns>
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	
	/// <summary>
	/// GPUデスクリプタハンドルを取得
	/// </summary>
	/// <param name="descriptorHeap">デスクリプタヒープ</param>
	/// <param name="descriptorSize">デスクリプタサイズ</param>
	/// <param name="index">デスクリプタ番号</param>
	/// <returns>GPUデスクリプタハンドル</returns>
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	
	/// <summary>
	/// 深度ステンシルビューの初期化
	/// </summary>
	void InitializeDepthStencilView();
	
	/// <summary>
	/// フェンスの生成
	/// </summary>
	void CreateFence();
	
	/// <summary>
	/// ビューポート矩形の生成
	/// </summary>
	void CreateViewPort();
	
	/// <summary>
	/// シザリング矩形の生成
	/// </summary>
	void CreateScissorRect();
	
	/// <summary>
	/// DXCコンパイラの生成
	/// </summary>
	void CreateDxcCompiler();
	
	/// <summary>
	/// FPS固定初期化
	/// </summary>
	void InitializeFixFPS();
	
	/// <summary>
	/// FPS固定更新
	/// </summary>
	void UpdateFixFPS();

private:	// シングルトンインスタンス
	static DirectXBase* sInstance;

	DirectXBase() = default;
	~DirectXBase() = default;
	DirectXBase(DirectXBase&) = delete;
	DirectXBase& operator=(DirectXBase&) = delete;

private:	// メンバ変数
	// DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
	// DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	// オフセット
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResource_ = nullptr;
	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	// 深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;
	// RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
	// DSV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
	// バックバッファ
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffer_;
	// 取得したRTVハンドル
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, 3> rtvHandles_ ;
	// Fence
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
	// FenceのSignal
	uint64_t fenceValue_;
	HANDLE fenceEvent_;
	// ビューポート矩形
	D3D12_VIEWPORT viewport_;
	// シザー矩形
	D3D12_RECT scissorRect_;
	// DXCユーティリティ
	IDxcUtils* dxcUtils_ = nullptr;
	// DXCコンパイラ
	IDxcCompiler3* dxcCompiler_ = nullptr;
	// デフォルトインクルードハンドラ
	IDxcIncludeHandler* includeHandler_ = nullptr;

	// WindowsAPI
	WindowsAPI* winApi_ = nullptr;
	
	// RTV用のDescriptorSize
	uint32_t descriptorSizeRTV_;
	// DSV用のDescriptorSIze
	uint32_t descriptorSizeDSV_;

	// offScreen用のIndex
	uint32_t offScreenSrvIndex_;
	D3D12_CPU_DESCRIPTOR_HANDLE offScreenSrvHandleCPU_;        // SRV作成時に必要なCPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE offScreenSrvHandleGPU_;        // 描画コマンドに必要なGPUハンドル

	D3D12_CLEAR_VALUE clearValue_;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;

	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

#pragma region 定数
	static constexpr uint32_t kBackBuffferCount = 2;
	static constexpr uint32_t kMaxRTVCount = 100;

	static const float kDefaultClearColor[4];

	static constexpr uint32_t kTargetFPS = 60;
	static constexpr uint32_t kFPSCheckMargin = 65;
	static constexpr uint64_t kMicroSecondsPerSecond = 1000000;
#pragma endregion
};