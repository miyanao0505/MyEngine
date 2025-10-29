#pragma once
#include "AbstractSceneFactory.h"

/// <summary>
/// ゲーム専用シーン工場クラス
/// AbstractSceneFactory を継承し、ゲーム内で使用する具体的なシーンオブジェクトの生成を担当します。
/// </summary>
class SceneFactory : public AbstractSceneFactory
{
public:	// メンバ関数
	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns>生成したシーン</returns>
	BaseScene* CreateScene(const std::string& sceneName) override;
};

