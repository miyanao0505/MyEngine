#include "TitleScene.h"
#include <imgui.h>
#include "CameraManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include"SceneManager.h"
#include "MyTools.h"

// 初期化
void TitleScene::Initialize()
{
	BaseScene::Initialize();

#pragma region カメラ
	CameraManager::GetInstance()->FindCamera("default");
	CameraManager::GetInstance()->GetCamera()->SetTranslate({ 0.0f, 20.0f, -40.0f });
#pragma endregion カメラ

#pragma region スプライト
	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(titleTextureFilePath_);
	// テスト用テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(skyBoxFilePath_);

	// スプライト
	titleSprite_ = std::make_unique<Sprite>();
	titleSprite_->Initialize(titleTextureFilePath_);
	titleSprite_->SetPosition({ 0.0f, 0.0f });	// スプライトの位置を設定

#pragma endregion スプライト

#pragma region Skybox
	// Skybox
	skybox_ = std::make_unique<Skybox>();
	skybox_->Initislize(skyBoxFilePath_, { 50.0f, 50.0f, 50.0f });
#pragma endregion Skybox

#pragma region 3Dオブジェクト
	// .objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel("debug/hummer", "hummer.obj");

	// 3Dオブジェクト
	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize({ 0.0f, 0.0f, 0.0f });
	player_->GetObject3d()->GetModel()->SetEnvironmentTexture(skyBoxFilePath_);

	// 敵
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();
	enemy_->GetObject3d()->GetModel()->SetEnvironmentTexture(skyBoxFilePath_);

#pragma endregion 3Dオブジェクト

#pragma region パーティクル
	// パーティクル
	/*particleEmitter_ = std::make_unique<ParticleEmitter>();
	particleEmitter_->Initialize("circle", "resources/circle.png");*/

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
	isParticleActive_ = true;
	isAccelerationField_ = false;
	acceleration_ = { 15.0f, 0.0f, 0.0f };
	area_ = { .min{-1.0f, -1.0f, -1.0f}, .max{1.0f, 1.0f, 1.0f} };
#pragma endregion 変数
}

// 終了
void TitleScene::Finalize()
{
	BaseScene::Finalize();

	// Skybox
	skybox_.reset();

	// 3Dオブジェクト
	for (auto& obj : testObjects_) {
		obj.reset();
	}

	// スプライト
	titleSprite_.reset();
}

// 毎フレーム更新
void TitleScene::Update()
{
	BaseScene::Update();

#ifdef _DEBUG
	// Nキーを押したら
	if (input_->TriggerKey(DIK_N)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
	// Bキーを押したら
	if (input_->TriggerKey(DIK_B)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("EVENT");
	}

	DebugDraw();

#endif // _DEBUG

	// Skyboxの更新
	skybox_->Update();

	// 3Dオブジェクトの更新処理
	// プレイヤーの更新処理
	player_->Update();

	// 敵の更新処理
	enemy_->Update();

	// test
	for (auto& obj : testObjects_) {
		obj->Update();
	}

	if (isAccelerationField_) {
		for (std::pair<const std::string, std::unique_ptr<ParticleManager::ParticleGroup>>& pair : ParticleManager::GetInstance()->GetParticleGroups()) {
			ParticleManager::ParticleGroup& group = *pair.second;
			int index = 0;
			for (std::list<MyBase::Particle>::iterator it = group.particles.begin(); it != group.particles.end();) {
				MyBase::Particle& particle = *it;

				if (MyTools::IsCollision(area_, particle.transform.translate)) {
					particle.velocity = MyTools::Add(particle.velocity, MyTools::Multiply(kDeltaTime_, acceleration_));
				}

				++it;
				++index;
			}
		}
	}

	// パーティクルの更新処理
	//particleEmitter_->Update();
	ParticleManager::GetInstance()->Update();

	// スプライトの更新処理
	titleSprite_->Update();
}

// 描画
void TitleScene::Draw()
{
#pragma region Skybox
	// 天球の描画
	skybox_->Draw();
#pragma endregion Skybox

#pragma region 3Dオブジェクト

	// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	ModelManager::GetInstance()->SetCommonScreen();

	// 全ての3DObject個々の描画
	// 敵の描画
	enemy_->Draw();

	// プレイヤーの描画
	player_->Draw();

	// test
	for (auto& obj : testObjects_) {
		obj->Draw();
	}

#pragma endregion 3Dオブジェクト

#pragma region パーティクル

	// パーティクルの描画準備。パーティクルの描画に共通グラフィックスコマンドを積む
	//ParticleManager::GetInstance()->Draw();

#pragma endregion パーティクル

#pragma region スプライト

	// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	TextureManager::GetInstance()->SetCommonScreen();

	// 全てのSprite個々の描画
	titleSprite_->Draw();

#pragma endregion スプライト
}

#ifdef _DEBUG
// デバッグ描画
void TitleScene::DebugDraw()
{
	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::SetNextWindowPos(ImVec2(20, 350), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Title Scene");
	ImGui::Text("N key : gameScene");
	ImGui::Text("B key : eventScene");
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(900, 20), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Settings");
	// Camera
	CameraManager::GetInstance()->DebugDraw();

	// Skybox
	skybox_->DebugDraw();

	// プレイヤー
	player_->DebugDraw();

	// 敵
	enemy_->DebugDraw();

	int num = 0;
	// test
	for (auto& obj : testObjects_) {
		ImGui::PushID(obj.get() + num);
		if (ImGui::CollapsingHeader("testObj")) {
			obj->DebugDraw();
		}
		ImGui::PopID();
		num++;
	}

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
		// オブジェクトの種類ごとに処理
		if (objectData.name == "Player") {
			// プレイヤーの初期化
			player_->GetObject3d()->SetModel(objectData.objectName);
			player_->SetName(objectData.name.c_str());
			player_->GetObject3d()->SetTranslate(objectData.translation);
			player_->GetObject3d()->SetRotate(objectData.rotation);
			player_->GetObject3d()->SetScale(objectData.scale);
		}
		else if (objectData.name == "Enemy") {
			// 敵の初期化
			enemy_->GetObject3d()->SetModel(objectData.objectName);
			enemy_->SetName(objectData.name.c_str());
			enemy_->GetObject3d()->SetTranslate(objectData.translation);
			enemy_->GetObject3d()->SetRotate(objectData.rotation);
			enemy_->GetObject3d()->SetScale(objectData.scale);
			for (const auto& childData : objectData.children) {
				ModelManager::GetInstance()->LoadModel("debug/hummer", "hummer.obj");
				TextureManager::GetInstance()->LoadTexture("resources/texture/hummer.png");
				BaseObject* childObj = CreateObjectFromData(childData);
				childObj->GetObject3d()->GetWorldTransform()->SetParent(enemy_->GetObject3d()->GetWorldTransform());
				testObjects_.emplace_back(childObj);
			}
		}
		else if (objectData.name == "Skydome") {
			// 天球の初期化

		}
		else {
			// その他のObjectはBaseObjectとして構築
			ModelManager::GetInstance()->LoadModel("debug/hummer", "hummer.obj");
			TextureManager::GetInstance()->LoadTexture("resources/texture/hummer.png");
			BaseObject* obj = CreateObjectFromData(objectData);
			testObjects_.emplace_back(obj);
			for (const auto& childData : objectData.children) {
				BaseObject* childObj = CreateObjectFromData(childData);
				childObj->GetObject3d()->GetWorldTransform()->SetParent(obj->GetObject3d()->GetWorldTransform());
				testObjects_.emplace_back(childObj);
			}
		}
	}
}
