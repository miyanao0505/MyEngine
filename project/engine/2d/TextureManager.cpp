#include "TextureManager.h"

using namespace DirectX;
using namespace StringUtility;
using namespace std;

TextureManager* TextureManager::instance = nullptr;

// シングルトンインスタンスの取得
TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return instance;
}

// 終了
void TextureManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

// 初期化
void TextureManager::Initialize(SrvManager* srvManager)
{
	dxBase_ = DirectXBase::GetInstance();

	srvManager_ = srvManager;

	spriteBase_ = make_unique<SpriteBase>();
	spriteBase_->Initialize(dxBase_);

	// SRVの数と同数
	textureDatas_.reserve(SrvManager::kMaxSRVCount);
}

void TextureManager::LoadTexture(const string& filePath)
{
	// 読み込み済みテクスチャを検索
	if (textureDatas_.contains(filePath)) {
		return;
	}

	// テクスチャ枚数上限チェック
	assert(srvManager_->isSecure());

	// Textureを読んで転送する
	ScratchImage mipImages = dxBase_->LoadTexture(filePath);

	// 追加したテクスチャデータの参照を取得する
	TextureData& textureData = textureDatas_[filePath];

	// テクスチャデータの書き込み
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = dxBase_->CreateTextureResource(textureData.metadata);
	textureData.intermediateResource = dxBase_->UploadTextureData(textureData.resource.Get(), mipImages);

	// SRV確保
	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

	// キューブマップか2Dか判定してSRV作成
	if ((textureData.metadata.miscFlags & TEX_MISC_TEXTURECUBE) != 0) {
		// キューブマップとしてSRV作成
		srvManager_->CreateSRVforTextureCube(textureData.srvIndex, textureData.metadata, textureData.resource.Get());
	}
	else {
		// 通常の2Dテクスチャ
		srvManager_->CreateSRVforTexture2D(textureData.srvIndex, textureData.metadata, textureData.resource.Get());
	}
}

// SRVインデックスの開始番号
uint32_t TextureManager::GetSrvIndex(const string& filePath)
{
	// 読み込み済みテクスチャを検索
	if (textureDatas_.contains(filePath)) {
		// 読み込み済みなら要素番号を返す
		uint32_t textureIndex = static_cast<uint32_t>(distance(textureDatas_.begin(), textureDatas_.end()));
		return textureIndex;
	}
	assert(0);
	return 0;
}

// テクスチャ番号からGPUハンドルを取得
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const string& filePath)
{
	// 範囲外指定違反チェック
	assert(srvManager_->isSecure());
	TextureData& textureData = textureDatas_[filePath];
	return textureData.srvHandleGPU;
}

// メタデータを取得
const DirectX::TexMetadata& TextureManager::GetMetaData(const string& filePath)
{
	// 範囲外指定違反チェック
	assert(srvManager_->isSecure());
	TextureData& textureData = textureDatas_[filePath];
	return textureData.metadata;
}

// ブレンドモードのセット
void TextureManager::SetBlendMode(SpriteBase::BlendMode blendMode)
{
	spriteBase_->SetBlendMode(blendMode);
	spriteBase_->CreateGraphicsPipeline();
}
