#include "TitleScene.h"
#include <imgui.h>
#include "CameraManager.h"
#include "LightManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include"SceneManager.h"
#include "MyBase.h"
#include "MyTools.h"

// 初期化
void TitleScene::Initialize()
{
	BaseScene::Initialize();

#pragma region カメラ
	CameraManager::GetInstance()->FindCamera("default");
	CameraManager::GetInstance()->GetCamera()->SetTranslate({ 0.0f, 0.0f, -40.0f });
	CameraManager::GetInstance()->GetCamera()->SetRotate({ 0.0f, 0.0f, 0.0f });
#pragma endregion カメラ

#pragma region ライト
	MyBase::PointLight pointLight;
	pointLight.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	pointLight.position = { 0.0f, 0.0f, -40.0f };
	pointLight.intensity = 1.0f;
	pointLight.radius = 100.0f;
	pointLight.decay = 2.0f;
	LightManager::GetInstance()->SetPointLight(pointLight);
#pragma endregion ライト

#pragma region スプライト
	// テクスチャの読み込み


	// スプライト
	

#pragma endregion スプライト

#pragma region Skydome
	// Skydome
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeFilePath_, { 0.0f, 0.0f, 0.0f }, {100.0f, 100.0f, 100.0f});
#pragma endregion Skybox

#pragma region 3Dオブジェクト
	// .objファイルからモデルを読み込む

	// 3Dオブジェクト
	titleLogo_ = std::make_unique<TitleLogo>();
	titleLogo_->Initialize();
	enterCharacters_ = std::make_unique<Characters>();
	enterCharacters_->Initialize("charactors", "enter.obj");

#pragma endregion 3Dオブジェクト

#pragma region パーティクル
	// パーティクル
	

#pragma endregion パーティクル

#pragma region jsonローダー
	// jsonローダー
	jsonLoader_ = std::make_unique<JsonLoader>();
	LoadJsonFile("gameScene.json");
#pragma endregion jsonローダー

#pragma region オーディオ
	// BGM

	
#pragma endregion オーディオ

#pragma region 変数
	
#pragma endregion 変数
}

// 終了
void TitleScene::Finalize()
{
	BaseScene::Finalize();
	
	// 3Dオブジェクト
	enterCharacters_.reset();
	titleLogo_.reset();
	skydome_.reset();
	
	// スプライト
	
}

// 毎フレーム更新
void TitleScene::Update()
{
	BaseScene::Update();

#ifdef _DEBUG
	DebugDraw();
#endif // _DEBUG

	// ゲームシーンへの遷移
	if (input_->TriggerKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("GAME");
		return;
	}

	// Skyboxの更新
	skydome_->Update();

	// 3Dオブジェクトの更新処理
	titleLogo_->Update();
	enterCharacters_->Update();

	// パーティクルの更新処理
	ParticleManager::GetInstance()->Update();

	// スプライトの更新処理
	
}

// 描画
void TitleScene::Draw()
{
#pragma region 3Dオブジェクト

	// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	ModelManager::GetInstance()->SetCommonScreen();

	// 全ての3DObject個々の描画
	skydome_->Draw();
	titleLogo_->Draw();
	enterCharacters_->Draw();

#pragma endregion 3Dオブジェクト

#pragma region パーティクル

	// パーティクルの描画準備。パーティクルの描画に共通グラフィックスコマンドを積む
	//ParticleManager::GetInstance()->Draw();

#pragma endregion パーティクル

#pragma region スプライト

	// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	TextureManager::GetInstance()->SetCommonScreen();

	// 全てのSprite個々の描画
	

#pragma endregion スプライト
}

#ifdef _DEBUG
// デバッグ描画
void TitleScene::DebugDraw()
{
	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::SetNextWindowPos(ImVec2(900, 20), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Settings"); 
	// Camera
	CameraManager::GetInstance()->DebugDraw();

	// Light
	LightManager::GetInstance()->DebugDraw();

	// 3Dオブジェクト
	skydome_->DebugDraw();

	// スプライト
	

	ImGui::End();
}
#endif // _DEBUG

// jsonファイルの読み込み
void TitleScene::LoadJsonFile(const std::string& filePath)
{
	// レベルデータの読み込み
	LevelData* levelData = jsonLoader_->LoadFile(filePath);

	// 3Dオブジェクトの読み込み
	for (const ObjectData& objectData : levelData->objects) {
		objectData;
	}
}
