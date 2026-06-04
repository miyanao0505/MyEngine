#pragma once
#include <string>
#include <d3d12.h>
#include <unordered_map>
#include "DirectXBase.h"
#include "SrvManager.h"
#include "DirectXTex.h"
#include "SpriteBase.h"

/// <summary>
/// DirectX 12 を用いたテクスチャ管理を行うクラス。
/// ゲームエンジン全体で利用されるテクスチャを一元的に管理する
/// Singleton マネージャー。
/// </summary>
class TextureManager
{
private: // テクスチャデータ構造体
	struct TextureData {
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

public:	// メンバ関数
	/// <summary>
	/// Singleton Instance を取得
	/// </summary>
	/// <returns>TextureManager</returns>
	static TextureManager* GetInstance();

	/// ------ Passkey Idion ------
	/// コントラクタを渡すための鍵
	class ConstructorKey {
	private:
		ConstructorKey() = default;
		friend class TextureManager;
	};

	/// PassKeyを受け取るコンストラクタ
	explicit TextureManager(ConstructorKey) {}

	/// コピー禁止
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	/// <summary>
	/// TextureManager を終了(解放)
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="srvManager">SRV (Shader Resource View) を管理する SrvManager へのポインタ</param>
	void Initialize(SrvManager* srvManager);

	/// <summary>
	/// 指定したテクスチャファイルを読み込み、内部で管理
	/// </summary>
	/// <param name="filePath">読み込むテクスチャファイルのパス</param>
	void LoadTexture(const std::string& filePath);

public: // getter
	/// <summary>
	/// 指定したテクスチャの SRV インデックスを取得
	/// </summary>
	/// <param name="filePath">SRV インデックスを取得するテクスチャのファイルパス</param>
	/// <returns>SRV インデックス</returns>
	uint32_t GetSrvIndex(const std::string& filePath);

	/// <summary>
	/// 指定したテクスチャの GPU 用 SRV ハンドルを取得
	/// </summary>
	/// <param name="filePath">GPU ハンドルを取得するテクスチャのファイルパス</param>
	/// <returns>D3D12_GPU_DESCRIPTOR_HANDLE 型の GPU ハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& filePath);

	/// <summary>
	/// 指定したテクスチャのメタデータを取得
	/// </summary>
	/// <param name="filePath">メタデータを取得するテクスチャのファイルパス</param>
	/// <returns>DirectX::TexMetadata 型のメタデータへの参照</returns>
	const DirectX::TexMetadata& GetMetaData(const std::string& filePath);

	/// <summary>
	/// 内部で管理している SpriteBase を取得
	/// </summary>
	/// <returns>SpriteBase へのポインタ</returns>
	SpriteBase* GetSpriteBase() const { return spriteBase_.get(); }

public:	// setter
	/// <summary>
	/// 共通の画面設定を SpriteBase に反映
	/// </summary>
	void SetCommonScreen() { spriteBase_->SetCommonScreen();}

	/// <summary>
	/// スプライト描画時のブレンドモードを設定
	/// </summary>
	/// <param name="blendMode">適用するブレンドモード (SpriteBase::BlendMode 列挙型)</param>
	void SetBlendMode(SpriteBase::BlendMode blendMode);

private: // Singleton Instance
	static std::unique_ptr<TextureManager> sInstance_;

private: // メンバ変数
	// テクスチャデータ
	// ★永続Map：読み込んだテクスチャをアプリ終了まで保持し、
	// ・読み込んだテクスチャをアプリケーション終了まで保持するキャッシュ
	// ・同じパスの読み込み時は再ロードせず高速化
	// ・テクスチャ削除は行わない(メモリ管理はアプリ全体の設計に依存)
	std::unordered_map<std::string, TextureData> textureDatas_;

	// DirectXBase
	DirectXBase* dxBase_ = nullptr;

	// SrvManager
	SrvManager* srvManager_ = nullptr;

	// スプライト共通部
	std::unique_ptr<SpriteBase> spriteBase_ = nullptr;
};