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

	// ▼ 起動直後（scene_がnullptr）の場合は即シーン切り替え
	if (!scene_ && nextScene_) {
		scene_ = nextScene_;
		nextScene_ = nullptr;
		scene_->SetSceneManager(this);
		scene_->Initialize();
		return;
	}

	// ▼ フェード演出中の更新
	if (transition_.IsActive()) {
		transition_.Update();
		return; // フェード中はこれで抜ける
	}

	// ▼ 次のシーンが予約されている場合、フェードアウト開始
	if (nextScene_) {
		// シーン切替演出を開始
		transition_.Start([this]() {
			// ▼ シーン切替完了後の処理
			if (scene_) {
				scene_->Finalize();
				delete scene_;
				scene_ = nullptr;
			}
			scene_ = nextScene_;
			nextScene_ = nullptr;
			scene_->SetSceneManager(this);
			scene_->Initialize();
		});
		return; // ここでフェード演出開始、まだ切り替えは完了していない
	}

	// ▼ 通常更新
	if (scene_) {
		scene_->Update();
	}
}

// 描画
void SceneManager::Draw()
{
	if (scene_) scene_->Draw();

	// フェード描画（上に重ねる）
	if (transition_.IsActive()) transition_.Draw();
}

/// 次のシーン予約
void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// 次のシーンを生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}
