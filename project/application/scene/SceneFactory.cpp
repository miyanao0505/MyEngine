#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "GameOverScene.h"
#include "EventScene.h"

SceneFactory::SceneFactory()
{
    createTable_.emplace(SceneName::Title, [] { return std::make_unique<TitleScene>(); });
    createTable_.emplace(SceneName::Game, [] { return std::make_unique<GameScene>(); });
    createTable_.emplace(SceneName::Clear, [] { return std::make_unique<ClearScene>(); });
    createTable_.emplace(SceneName::GameOver, [] { return std::make_unique<GameOverScene>(); });
    createTable_.emplace(SceneName::Event, [] { return std::make_unique<EventScene>(); });
}

/// SceneNameに対応するシーンインスタンスを生成する
std::unique_ptr<BaseScene> SceneFactory::CreateScene(SceneName sceneName)
{
	auto it = createTable_.find(sceneName);
	if (it == createTable_.end()) {
		return nullptr;
	}

	return it->second();
}
