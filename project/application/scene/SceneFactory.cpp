#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "ClearScene.h"
#include "EventScene.h"

/// シーン生成
BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    // 次のシーンを生成
    BaseScene* newScene = nullptr;

    if (sceneName == "TITLE") {
        newScene = new TitleScene();
    }
    else if (sceneName == "GAME") {
        newScene = new GameScene();
    }
    else if (sceneName == "CLEAR") {
        newScene = new ClearScene();
    }
	else if (sceneName == "GAMEOVER") {
		newScene = new GameOverScene();
	}
	else if (sceneName == "EVENT") {
		newScene = new EventScene();
	}
	else {
		// シーン名が不正な場合はnullptrを返す
		return nullptr;
	}

    return newScene;
}
