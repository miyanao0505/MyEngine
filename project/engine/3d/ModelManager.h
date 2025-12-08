#pragma once
#include <map>
#include <memory>
#include <string>
#include "Object3dBase.h"
#include "ModelBase.h"
#include "Model.h"

/// <summary>
/// 3Dモデルの読み込み・管理を行うシングルトンクラス。
/// ModelManager クラスは、ゲーム内で使用される全ての 3D モデルを一元的に管理します。
/// </summary>
class ModelManager
{
public:	// メンバ関数
	/// <summary>
	/// シングルトンインスタンスを取得
	/// </summary>
	/// <returns>ModelManager の唯一のインスタンス</returns>
	static ModelManager* GetInstance();
	
	/// <summary>
	/// ModelManager を終了(解放)
	/// </summary>
	void Finalize();

	/// <summary>
	///	初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// モデルファイルを読み込み、管理リストに登録
	/// </summary>
	/// <param name="directoryPath">モデルファイルが格納されているディレクトリパス</param>
	/// <param name="filePath">読み込むモデルファイル名(.objなど)</param>
	void LoadModel(const std::string& directoryPath, const std::string& filePath);

	/// <summary>
	/// 指定したパスのモデルを管理リストから検索
	/// </summary>
	/// <param name="filePath">検索するモデルファイルのパス</param>
	/// <returns>Model インスタンスへのポインタ。該当モデルが存在しない場合は nullptr を返す</returns>
	Model* FindModel(const std::string& filePath);

	/// <summary>
	/// 共通の画面設定を Object3dBase に反映
	/// </summary>
	void SetCommonScreen() { object3dBase_->SetCommonScreen(); }

public:	// getter
	/// <summary>
	/// 内部で管理している Object3dBase を取得
	/// </summary>
	/// <returns>Object3dBase へのポインタ</returns>
	Object3dBase* GetObject3dBase() const { return object3dBase_.get(); }

public:	// setter
	/// <summary>
	/// モデル描画のブレンドモードを設定
	/// </summary>
	/// <param name="blendMode">適用するブレンドモード (Object3dBase::BlendMode 列挙型)</param>
	void SetBlendMode(Object3dBase::BlendMode blendMode);

private:	// シングルトン
	static ModelManager* instance;

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;

private:	// メンバ変数
	// モデルデータ
	std::map<std::string, std::unique_ptr<Model>> models_;

	std::unique_ptr<ModelBase> modelBase_ = nullptr;
	std::unique_ptr<Object3dBase> object3dBase_ = nullptr;
};

