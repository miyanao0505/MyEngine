#include "TextureManager.h"
#include "AssetPath.h"

using namespace DirectX;
using namespace StringUtility;
using namespace std;

/// static member 定義
unique_ptr<TextureManager> TextureManager::sInstance_ = nullptr;

// Singleton Instance を取得
TextureManager* TextureManager::GetInstance() {
	if (sInstance_ == nullptr) {
		sInstance_ = make_unique<TextureManager>(TextureManager::ConstructorKey{});
	}
	return sInstance_.get();
}

// 終了
void TextureManager::Finalize() {
	sInstance_.reset();
}

// 初期化
void TextureManager::Initialize(SrvManager* srvManager) {
	// DirectXBaseの取得
	dxBase_ = DirectXBase::GetInstance();

	// 引数を受け取ってメンバ変数に記録する
	srvManager_ = srvManager;

	// スプライト描画基盤の生成
	spriteBase_ = make_unique<SpriteBase>();
	spriteBase_->Initialize(dxBase_);

	// SRVの数と同数
	textureDatas_.reserve(SrvManager::kMaxSRVCount);
}

void TextureManager::LoadTexture(const string& filePath) {
	// すでに読み込まれている場合は早期リターン
	if (textureDatas_.contains(filePath)) return;
	
	// テクスチャ枚数上限チェック
	assert(srvManager_->IsSecure());

	// フルパス作成
	string fullPath = AssetPath::kTextureRootPath + filePath;

	// Textureを読んで転送する
	ScratchImage mipImages = dxBase_->LoadTexture(fullPath);

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

	// キューブマップか2Dか判定して適切なSRV作成
	if ((textureData.metadata.miscFlags & TEX_MISC_TEXTURECUBE) != 0) {
		srvManager_->CreateSRVForTextureCube(textureData.srvIndex, textureData.metadata, textureData.resource.Get());
	} else {
		srvManager_->CreateSRVForTexture2D(textureData.srvIndex, textureData.metadata, textureData.resource.Get());
	}
}

// SRVインデックスの開始番号
uint32_t TextureManager::GetSrvIndex(const string& filePath) {
	auto it = textureDatas_.find(filePath);
	assert(it != textureDatas_.end()); // 未ロードはバグ

	return it->second.srvIndex;
}

// テクスチャ番号からGPUハンドルを取得
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const string& filePath) {
	auto it = textureDatas_.find(filePath);
	assert(it != textureDatas_.end()); // 未ロードはバグ

	return it->second.srvHandleGPU;
}

// メタデータを取得
const DirectX::TexMetadata& TextureManager::GetMetaData(const string& filePath) {
	auto it = textureDatas_.find(filePath);
	assert(it != textureDatas_.end()); // 未ロードはバグ

	return it->second.metadata;
}

// ブレンドモードのセット
void TextureManager::SetBlendMode(SpriteBase::BlendMode blendMode) {
	// スプライト共通部にブレンドモードを設定し、パイプラインを再構築
	spriteBase_->SetBlendMode(blendMode);
	spriteBase_->CreateGraphicsPipeline();
}
