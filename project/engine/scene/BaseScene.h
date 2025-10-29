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
	// デバック描画
	virtual void DebugDraw();
#endif // _DEBUG

	// オブジェクト構築
	BaseObject* CreateObjectFromData(const ObjectData& data);
	// jsonファイルの読み込み
	virtual void LoadJsonFile(const std::string& filePath) = 0;

public:	// setter
	// シーンマネージャのセット
	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

protected:	// 継承
	// シーンマネージャ(借りてくる)
	SceneManager* sceneManager_ = nullptr;
	// 入力
	Input* input_ = nullptr;

};

