#include "ModelManager.h"

using namespace std;

ModelManager* ModelManager::instance = nullptr;

// シングルトンインスタンスの取得
ModelManager* ModelManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ModelManager;
	}
	return instance;
}

// 終了
void ModelManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

// 初期化
void ModelManager::Initialize()
{
	// モデル共通部の初期化
	modelBase_ = make_unique<ModelBase>();
	modelBase_->Initislize();

	// 3dオブジェクト共通部の初期化
	object3dBase_ = make_unique<Object3dBase>();
	object3dBase_->Initislize();
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


void ModelManager::SetBlendMode(Object3dBase::BlendMode blendMode)
{
	object3dBase_->SetBlendMode(blendMode);
	object3dBase_->CreateGraphicsPipeline();
}
