#pragma once
#include <memory>
#include"Input.h"
#include "JsonLoader.h"
#include "BaseObject.h"

// 前方宣言
class SceneManager;

// シーン基底クラス
class BaseScene
{
public:	//メンバ関数
	virtual ~BaseScene() = default;

	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 更新
	virtual void Update();

	// 描画
	virtual void Draw();

	// デバック描画
	virtual void DebugDraw();

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

