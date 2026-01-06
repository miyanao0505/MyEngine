#include "SceneManager.h"
#include <cassert>
#include <imgui.h>

namespace {
	constexpr float kDebugSceneWindowPosX = 20.0f;
	constexpr float kDebugSceneWindowPosY = 20.0f;
	constexpr float kDebugSceneWindowWidth = 350.0f;
	constexpr float kDebugSceneWindowHeight = 150.0f;
}

SceneManager* SceneManager::sInstance = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (sInstance == nullptr) {
		sInstance = new SceneManager();
	}
	return sInstance;
}

// 終了
void SceneManager::Finalize()
{
	scene_.reset();

	delete sInstance;
	sInstance = nullptr;
}

// 更新
void SceneManager::Update()
{
#ifdef _DEBUG
	ImGui::SetNextWindowPos(ImVec2(kDebugSceneWindowPosX, kDebugSceneWindowPosY), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(kDebugSceneWindowWidth, kDebugSceneWindowHeight), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)
	ImGui::Begin("scene");
	if (ImGui::Button("Title")) {
		nextScene_ = sceneFactory_->CreateScene(SceneName::Title);
	}
	if (ImGui::Button("Game")) {
		nextScene_ = sceneFactory_->CreateScene(SceneName::Game);
	}
	if (ImGui::Button("Clear")) {
		nextScene_ = sceneFactory_->CreateScene(SceneName::Clear);
	}
	if (ImGui::Button("GameOver")) {
		nextScene_ = sceneFactory_->CreateScene(SceneName::GameOver);
	}
	ImGui::End();
#endif // _DEBUG

	// ▼ 起動直後（scene_がnullptr）の場合は即シーン切り替え
	if (!scene_ && nextScene_) {
		scene_ = std::move(nextScene_);
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
			scene_.reset(); // 古いシーンを終了・解放
			scene_ = std::move(nextScene_);
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
void SceneManager::ChangeScene(SceneName sceneName)
{
	assert(sceneFactory_);
	assert(!nextScene_);

	// 次のシーンを生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}
