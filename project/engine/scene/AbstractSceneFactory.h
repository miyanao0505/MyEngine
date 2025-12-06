#pragma once
#include "BaseScene.h"
#include "SceneName.h"

/// <summary>
/// シーンファクトリー抽象クラス(AbstractSceneFactory)
/// ゲーム内のシーンを生成するためのインターフェースを定義する抽象クラスです。
/// </summary>
class AbstractSceneFactory
{
public:	// メンバ関数
	// 仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	
	/// <summary>
	/// シーンの生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns>生成したシーンのポインタ</returns>
	virtual std::unique_ptr<BaseScene> CreateScene(SceneName sceneName) = 0;
};