#include "ModelManager.h"

using std::string;
using std::unique_ptr;
using std::make_unique;

ModelManager* ModelManager::sInstance = nullptr;

// シングルトンインスタンスの取得
ModelManager* ModelManager::GetInstance()
{
	if (sInstance == nullptr) {
		sInstance = new ModelManager;
	}
	return sInstance;
}

// 終了
void ModelManager::Finalize()
{
	delete sInstance;
	sInstance = nullptr;
}

// 初期化
void ModelManager::Initialize()
{
	// ModelBaseの生成と初期化
	modelBase_ = make_unique<ModelBase>();
	modelBase_->Initialize();

	// Object3dBaseの生成と初期化
	object3dBase_ = make_unique<Object3dBase>();
	object3dBase_->Initialize();
}

/// モデルファイルの読み込み
void ModelManager::LoadModel(const string& directoryPath, const string& filePath)
{
	// 連結してフルパスを得る
	const string fullpath = "resources/model/" + directoryPath;

	// 読み込み済みモデルを検索
	if (models_.contains(filePath)) {
		// 読み込み済みなら早期return
		return;
	}

	// モデルの生成とファイル読み込み、初期化
	unique_ptr<Model> model = make_unique<Model>();
	model->Initialize(modelBase_.get(), fullpath, filePath);

	// モデルをmapコンテナに格納する
	models_.insert(make_pair(filePath, move(model)));
}

/// モデルの検索
Model* ModelManager::FindModel(const string& filePath)
{
	// 読み込み済みモデルを検索
	if (models_.contains(filePath)) {
		// 読み込みモデルを戻り値としてreturn
		return models_.at(filePath).get();
	}

	// ファイル名一致なし
	return nullptr;
}

// モデル描画のブレンドモードを設定
void ModelManager::SetBlendMode(Object3dBase::BlendMode blendMode)
{
	// Object3dBase にブレンドモードを設定
	object3dBase_->SetBlendMode(blendMode);

	// グラフィックスパイプラインの再作成
	object3dBase_->CreateGraphicsPipeline();
}
