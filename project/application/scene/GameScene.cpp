#include "GameScene.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "AudioManager.h"
#include"SceneManager.h"
#include "TimeManager.h"
#include "MyTools.h"

using namespace std;

// 初期化
void GameScene::Initialize()
{
#pragma region シーン初期化
	BaseScene::Initialize();

#pragma region ライト
	LightManager::GetInstance()->Initialize();
#pragma endregion ライト

#pragma region スプライト
	// テクスチャの読み込み

	// スプライト

#pragma endregion スプライト

#pragma region 3Dオブジェクト
	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize({ 0.0f, 0.0f, 0.0f });

	// 敵
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();
	enemy_->SetPlayer(player_.get());

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize("skyback.png", { 0.0f, 0.0f, 0.0f }, {100.0f, 100.0f, 100.0f});
	
#pragma endregion 3Dオブジェクト

#pragma region カメラ
	// フォローカメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetPlayer(player_.get());
	// レールカメラ
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize({
		{ 0.0f, 0.0f, -50.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 50.0f },
		});
#pragma endregion カメラ

#pragma region シーケンス
	// シーケンス
	startSequence_ = std::make_unique<StartSequence>();
	startSequence_->Initialize();
#pragma endregion シーケンス

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

	// お試し用
	AudioManager::GetInstance()->LoadAudioWave("fanfare.wav");
#pragma endregion オーディオ

#pragma region 変数
	isParticleActive_ = true;
	isAccelerationField_ = false;
	acceleration_ = { 15.0f, 0.0f, 0.0f };
	area_ = { .min{-1.0f, -1.0f, -1.0f}, .max{1.0f, 1.0f, 1.0f} };

	isGameClear_ = false;
	gameClearTimer_ = 0.0f;
#pragma endregion 変数

	// 最初の更新
	railCamera_->Update(TimeManager::GetInstance()->GetDeltaTime());
	followCamera_->Update(TimeManager::GetInstance()->GetDeltaTime());
	CameraManager::GetInstance()->GetCamera()->Update();
	player_->Update(TimeManager::GetInstance()->GetDeltaTime());
	enemy_->Update(TimeManager::GetInstance()->GetDeltaTime());
	skydome_->Update();

#pragma endregion シーン初期化
}

// 終了
void GameScene::Finalize()
{
	jsonLoader_.reset();
	startSequence_.reset();
	for(std::unique_ptr<MyBase::PlayerSpawnData>& spawnPoint : spawnPoints_){
		spawnPoint.reset();
	}
	spawnPoints_.clear();
	railCamera_.reset();
	followCamera_.reset();

	// 3Dオブジェクト
	skydome_.reset();
	enemy_.reset();
	player_.reset();

	// スプライト

	BaseScene::Finalize();
}

// 毎フレーム更新
void GameScene::Update()
{
	BaseScene::Update();

#ifdef _DEBUG
	DebugUpdate();
#endif // _DEBUG

	// カメラマネージャーの更新
	CameraManager::GetInstance()->Update(TimeManager::GetInstance()->GetDeltaTime());
	// カメラの更新
	if (CameraManager::GetInstance()->GetCamera()) {
		CameraManager::GetInstance()->GetCamera()->Update();
	}

	// クリアフラグが立っている場合
	if (isGameClear_) {
		// クリアタイマー更新
		gameClearTimer_ -= TimeManager::GetInstance()->GetDeltaTime();
		// タイマーが0以下になったら
		if (gameClearTimer_ <= 0.0f) {
			// シーン切り替え依頼
			SceneManager::GetInstance()->ChangeScene(SceneName::Clear);
			return;
		}
		return;
	}

	// スタート演出中
	if (!startSequence_->IsFinished()) {
		startSequence_->Update(TimeManager::GetInstance()->GetDeltaTime());
		return;
	}
	
	// クリア条件
	if (enemy_->IsDead()) {
		// ゲームクリアフラグON
		isGameClear_ = true;
		// クリアタイマーセット
		gameClearTimer_ = kGameClearDuration;
		return;
	}

	// 3Dオブジェクトの更新処理
	// プレイヤーの更新処理
	player_->Update(TimeManager::GetInstance()->GetDeltaTime());

	// フォローカメラの更新
	followCamera_->Update(TimeManager::GetInstance()->GetDeltaTime());

	// レールカメラの更新
	railCamera_->Update(TimeManager::GetInstance()->GetDeltaTime());

	// 敵の更新処理
	enemy_->Update(TimeManager::GetInstance()->GetDeltaTime());

	// 天球の更新
	skydome_->Update();

	if (isAccelerationField_) {
		for (std::pair<const std::string, std::unique_ptr<ParticleManager::ParticleGroup>>& pair : ParticleManager::GetInstance()->GetParticleGroups()) {
			ParticleManager::ParticleGroup& group = *pair.second;
			int index = 0;
			for (std::list<MyBase::Particle>::iterator it = group.particles.begin(); it != group.particles.end();) {
				MyBase::Particle& particle = *it;

				if (MyTools::IsCollision(area_, particle.transform.translate)) {
					particle.velocity = MyTools::Add(particle.velocity, MyTools::Multiply(TimeManager::GetInstance()->GetDeltaTime(), acceleration_));
				}

				++it;
				++index;
			}
		}
	}

	// パーティクルの更新処理
	ParticleManager::GetInstance()->Update();

	// スプライトの更新処理
}

// 描画
void GameScene::Draw()
{
#pragma region 3Dオブジェクト

	// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	ModelManager::GetInstance()->SetCommonScreen();

	// 全ての3DObject個々の描画
	// シーケンスの描画
	if(!startSequence_->IsFinished()){
		startSequence_->Draw();
	}

	// 天球の描画
	skydome_->Draw();

	// 敵の描画
	enemy_->Draw();

	// プレイヤーの描画
	player_->Draw();

#pragma endregion 3Dオブジェクト

#pragma region パーティクル

	// パーティクルの描画準備。パーティクルの描画に共通グラフィックスコマンドを積む
	ParticleManager::GetInstance()->Draw();

#pragma endregion パーティクル

#pragma region スプライト

	// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	TextureManager::GetInstance()->SetCommonScreen();

	// 全てのSprite個々の描画

#pragma endregion スプライト
}

#ifdef _DEBUG
// デバッグ更新
void GameScene::DebugUpdate()
{
	// Nキーを押したら
	if (input_->IsKeyTriggered(DIK_N)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene(SceneName::Clear);
	}
	// Mキーを押したら
	if (input_->IsKeyTriggered(DIK_M)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene(SceneName::GameOver);
	}
	// Bキーを押したら
	if (input_->IsKeyTriggered(DIK_B)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene(SceneName::Event);
	}

	DebugDraw();
}

// デバッグ描画
void GameScene::DebugDraw()
{
	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::SetNextWindowPos(ImVec2(20, 350), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Game");
	ImGui::Text("N key : clearScene");
	ImGui::Text("M key : gameOverScene");
	ImGui::Text("B key : eventScene");
	ImGui::Text("\n");
	ImGui::Text("P key : particle On / Off");
	ImGui::Text("O key : acceleration On / Off");
	ImGui::Text("\n");
	ImGui::Text("K key : sampleAudio Play");
	ImGui::Text("U key : sampleAudio UnLoad");
	ImGui::Text("L key : sampleAudio Load");
	ImGui::End();

	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::SetNextWindowPos(ImVec2(900, 20), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Settings");
	// Camera
	CameraManager::GetInstance()->DebugDraw();
	// FollowCamera
	followCamera_->DebugDraw();
	// RailCamera
	railCamera_->DebugDraw();

	// Lighting
	LightManager::GetInstance()->DebugDraw();

	// Skybox

	// プレイヤー
	player_->DebugDraw();

	// 敵
	enemy_->DebugDraw();

	// パーティクル
	ParticleManager::GetInstance()->ImGui();

	ImGui::End();
}
#endif // _DEBUG

// JSONファイルの読み込み
void GameScene::LoadJsonFile([[maybe_unused]] const std::string& filePath)
{
	// レベルデータの読み込み
	std::unique_ptr<JsonLevelData> levelData = jsonLoader_->LoadFile(filePath);
	
	// 3Dオブジェクトの読み込み
	for (const JsonObjectData& objectData : levelData->objects) {
		// オブジェクトの種類ごとに処理
		if (objectData.name == "Player") {
			// プレイヤーの初期化
			player_->GetObject3D()->SetModel(objectData.modelFileName);
			player_->SetName(objectData.name.c_str());
			player_->GetObject3D()->SetTranslate(objectData.translation);
			player_->GetObject3D()->SetRotate(objectData.rotation);
			player_->GetObject3D()->SetScale(objectData.scale);
		}
		else if (objectData.name == "Enemy") {
			// 敵の初期化

		}
		else if (objectData.name == "Skydome") {
			// 天球の初期化

		}
		else {
			// その他のObjectはBaseObjectとして構築
		}
	}

	// スポーンポイントの読み込み
	for (const MyBase::PlayerSpawnData& spawnData : levelData->players) {
		spawnPoints_.push_back(std::make_unique<MyBase::PlayerSpawnData>(spawnData));
	}
}
