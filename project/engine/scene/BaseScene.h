#pragma once
#include <memory>
#include"Input.h"
#include "JsonLoader.h"
#include "BaseObject.h"

// 前方宣言
class SceneManager;

/// <summary>
/// シーン基底クラス(BaseScene)
/// ゲーム内のシーン(ステージ、タイトル画面、ゲームオーバー画面など)
/// の共通インターフェースと基本機能を提供する抽象クラスです。
/// </summary>
class BaseScene
{
public:	//メンバ関数
	virtual ~BaseScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

#ifdef _DEBUG
	/// <summary>
	/// デバック描画
	/// </summary>
	virtual void DebugDraw();
#endif // _DEBUG
	/// <summary>
	/// オブジェクト構築
	/// </summary>
	/// <param name="data">オブジェクトデータ</param>
	/// <returns>生成されたオブジェクトのポインタ</returns>
	BaseObject* CreateObjectFromData(const ObjectData& data);

	/// <summary>
	/// jsonファイルの読み込み
	/// </summary>
	///	<param name="filePath">ファイルパス</param>
	virtual void LoadJsonFile(const std::string& filePath) = 0;

public:	// setter
	/// <summary>
	/// シーンマネージャのセット
	/// </summary>
	/// <param name="sceneManager">シーンマネージャのポインタ</param>
	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

protected:	// 継承
	// シーンマネージャ(借りてくる)
	SceneManager* sceneManager_ = nullptr;
	// 入力
	Input* input_ = nullptr;

};

