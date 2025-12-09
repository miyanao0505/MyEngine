#pragma once
#include "AbstractSceneFactory.h"
#include <unordered_map>
#include <functional>

/// <summary>
/// ゲーム専用シーン工場クラス
/// AbstractSceneFactory を継承し、ゲーム内で使用する具体的なシーンオブジェクトの生成を担当します。
/// </summary>
class SceneFactory : public AbstractSceneFactory
{
public:	// メンバ関数
	SceneFactory();
	~SceneFactory() override = default;

	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns>生成したシーン</returns>
	std::unique_ptr<BaseScene> CreateScene(SceneName sceneName) override;

private:
	using CreateFunc = std::function<std::unique_ptr<BaseScene>()>;
	std::unordered_map<SceneName, CreateFunc> createTable_;
};