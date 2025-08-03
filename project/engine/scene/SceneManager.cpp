#include "SceneManager.h"
#include <cassert>
#include <imgui.h>

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new SceneManager();
	}
	return instance;
}

// 終了
void SceneManager::Finalize()
{
	// 最後のシーンの終了と解放
	scene_->Finalize();
	delete scene_;

	delete instance;
	instance = nullptr;
}

// 更新
void SceneManager::Update()
{
#ifdef _DEBUG
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)
	ImGui::Begin("scene");
	if (ImGui::Button("TitleScene")) {
		nextScene_ = sceneFactory_->CreateScene("TITLE");
	}
	if (ImGui::Button("GameScene")) {
		nextScene_ = sceneFactory_->CreateScene("GAME");
	}
	if (ImGui::Button("ClearScene")) {
		nextScene_ = sceneFactory_->CreateScene("CLEAR");
	}
	if (ImGui::Button("GameOverScene")) {
		nextScene_ = sceneFactory_->CreateScene("GAMEOVER");
	}
	ImGui::End();
#endif // _DEBUG


	// シーン切り替え機構
	// 次シーンの予約があるなら
	if (nextScene_) {
		// 旧シーンの終了
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}

		// シーンの切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;

		// シーンマネージャをセット
		scene_->SetSceneManager(this);

		// 次のシーンを初期化する
		scene_->Initialize();
	}

	// 実行中シーンを更新する
	scene_->Update();
}

// 描画
void SceneManager::Draw()
{
	scene_->Draw();
}

/// 次のシーン予約
void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// 次のシーンを生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}
