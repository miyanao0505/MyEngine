#include "EventScene.h"
#include "CameraManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "MyTools.h"
#include "DebugLineBase.h"

// 初期化
void EventScene::Initialize()
{
	BaseScene::Initialize();

#pragma region シーン初期化
	// テクスチャの読み込み

	// スプライト

	// .objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel("debug/cube", "cube.obj");

	// 3Dオブジェクト
	

	// パーティクル
#pragma endregion シーン初期化

#pragma region 変数
	
#pragma endregion 変数
}

// 終了
void EventScene::Finalize()
{
	BaseScene::Finalize();

	// 3Dオブジェクト
	

	// スプライト

}

// 毎フレーム更新
void EventScene::Update()
{
	BaseScene::Update();

#ifdef _DEBUG
	DebugUpdate();
#endif // _DEBUG

	// 3Dオブジェクトの更新処理
	

	// パーティクルの更新処理
	ParticleManager::GetInstance()->Update();

	// スプライトの更新処理

}

// 描画
void EventScene::Draw()
{
#pragma region 3Dオブジェクト

	// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	ModelManager::GetInstance()->SetCommonScreen();

	// 全ての3DObject個々の描画
	

#pragma endregion 3Dオブジェクト

#pragma region パーティクル

	// パーティクルの描画準備。パーティクルの描画に共通グラフィックスコマンドを積む

#pragma endregion パーティクル

#pragma region スプライト

	// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	TextureManager::GetInstance()->SetCommonScreen();

	// 全てのSprite個々の描画
	

#pragma endregion スプライト

}

#ifdef _DEBUG
// デバッグ更新
void EventScene::DebugUpdate()
{
	// Nキーを押したら
	if (input_->TriggerKey(DIK_N)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene(SceneName::Title);
	}

	DebugDraw();
}

// デバッグ描画
void EventScene::DebugDraw()
{
	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::SetNextWindowPos(kDebugWindowPos, ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(kDebugWindowSize, ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Event");
	ImGui::Text("N key : titleScene");
	ImGui::End();
}
#endif // _DEBUG

// jsonファイルの読み込み
void EventScene::LoadJsonFile([[maybe_unused]] const std::string& filePath)
{
	
}