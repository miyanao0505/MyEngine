#pragma once
#include "BaseScene.h"
#include <string>

/// <summary>
/// シーンファクトリー抽象クラス(AbstractSceneFactory)
/// ゲーム内のシーンを生成するためのインターフェースを定義する抽象クラスです。
/// </summary>
class AbstractSceneFactory
{
public:	// メンバ関数
	// 仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	// シーンの生成
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};

