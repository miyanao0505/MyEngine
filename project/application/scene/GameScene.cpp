#include "GameScene.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "AudioManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "MyTools.h"
#ifdef _DEBUG
#include "DebugLineBase.h"
#endif // _DEBUG

using namespace std;
using namespace MyBase;

#pragma region 定数
const Vector3 GameScene::kPlayerInitialTranslate{ 0.0f, 0.0f, 0.0f };
const vector<Vector3> GameScene::kEnemyInitialTranslates{
	{ 5.0f, 0.0f, 500.0f },
	{ 5.0f, 7.0f, 750.0f },
	{ -5.0f, 0.0f, 1000.0f },
	{ -5.0f, 7.0f, 1250.0f },
	{ 5.0f, -7.0f, 1500.0f },
	{ 5.0f, 7.0f, 1750.0f },
};
const vector<Vector3> GameScene::kRailPoints{
	{ 0, 0, 0 },
	{ 0, 0, 250 },
	{ 40, 20, 500 },
	{ -50, 0, 750 },
	{ 50, -50, 1000 },
	{ 0, 0, 1250 },
	{ 0, 0, 1500 },
	{ 0, 0, 2000 },
};
#pragma endregion

// 初期化
void GameScene::Initialize() {
#pragma region シーン初期化
	BaseScene::Initialize();

#pragma region ライト
	LightManager::GetInstance()->SetPointLightIntensity(0.0f);
	LightManager::GetInstance()->SetDirectionalLightIntensity(1.0f);
	LightManager::GetInstance()->SetSpotLightIntensity(0.0f);
#pragma endregion ライト

#pragma region スプライト
	// スプライト
	escapeUI_ = make_unique<Sprite>();
	escapeUI_->Initialize("EscButton.png");
	escapeUI_->SetPosition({ 20.0f, 20.0f });
	escapeUI_->SetSize({ 100.0f, 50.0f });
#pragma endregion スプライト

#pragma region 3Dオブジェクト
	// プレイヤー
	player_ = make_unique<Player>();
	player_->Initialize(kPlayerInitialTranslate);

	// 敵
	for(size_t i = 0; i < kEnemyInitialTranslates.size(); ++i){
		unique_ptr<Enemy> enemy = make_unique<Enemy>();
		enemy->Initialize();
		enemy->SetPlayer(player_.get());
		enemy->SetWorldPosition(kEnemyInitialTranslates[i]);
		enemies_.push_back(std::move(enemy));
	}

	// 天球
	skydome_ = make_unique<Skydome>();
	skydome_->Initialize("skyback.png", kSkydomeTranslate, kSkydomeScale);
	
#pragma endregion 3Dオブジェクト

#pragma region カメラ
	CameraManager::GetInstance()->AddCamera("FollowCamera");
	CameraManager::GetInstance()->SetCamera("FollowCamera");
	// フォローカメラ
	followCamera_ = make_unique<FollowCamera>();
	followCamera_->Initialize(CameraManager::GetInstance()->GetCamera());
	// レールカメラ
	railCamera_ = make_unique<RailCamera>();
	railCamera_->Initialize();
	railCamera_->SetRailPoints(kRailPoints);
	// レール追従システム
	railFollowSystem_ = make_unique<RailFollowSystem>();
	railFollowSystem_->Initialize(railCamera_.get(), followCamera_.get());
#pragma endregion カメラ

#pragma region シーケンス
	// シーケンス
	startSequence_ = make_unique<StartSequence>();
	startSequence_->Initialize();
#pragma endregion シーケンス

#pragma region ポーズ管理
	// ポーズコントローラ
	pauseController_ = make_unique<PauseController>();
	pauseController_->Initialize();
#pragma endregion

#pragma region デバッグ
#ifdef _DEBUG
	DebugLineBase::GetInstance()->Initialize(DirectXBase::GetInstance());
#endif // _DEBUG
#pragma endregion

#pragma region パーティクル
	// パーティクル
	
#pragma endregion パーティクル

#pragma region jsonローダー
	// jsonローダー
	jsonLoader_ = make_unique<JsonLoader>();
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
	acceleration_ = kAcceleration;
	area_ = kAccelArea;

	isGameClear_ = false;
	gameClearTimer_ = 0.0f;
#pragma endregion 変数

	// 最初の更新
	CameraManager::GetInstance()->GetCamera()->SetTranslate(kCameraTranslate);
	CameraManager::GetInstance()->GetCamera()->Update();
	escapeUI_->Update();
	player_->Update(TimeManager::GetInstance()->GetDeltaTime());
	for(unique_ptr<Enemy>& enemy : enemies_){
		enemy->Update(TimeManager::GetInstance()->GetDeltaTime());
	}
	skydome_->Update();
	followCamera_->SetTargetPosition(player_->GetWorldPosition());
	followCamera_->UpdateLookAtTarget();

#pragma endregion シーン初期化
}

// 終了
void GameScene::Finalize() {
	jsonLoader_.reset();
	startSequence_.reset();
	for(unique_ptr<PlayerSpawnData>& spawnPoint : spawnPoints_){
		spawnPoint.reset();
	}
	spawnPoints_.clear();
	railCamera_.reset();
	followCamera_.reset();

	// 3Dオブジェクト
	skydome_.reset();
	for(unique_ptr<Enemy>& enemy : enemies_){
		enemy.reset();
	}
	player_.reset();

	// スプライト
	escapeUI_.reset();
	
#ifdef _DEBUG
	DebugLineBase::Finalize();
#endif // _DEBUG

	BaseScene::Finalize();
}

// 毎フレーム更新
void GameScene::Update()
{
	BaseScene::Update();

#ifdef _DEBUG
	// デバッグ更新
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
		}
		return;
	}

	// ゲームオーバーフラグが立っている場合
	if (isGameOver_) {
		// ゲームオーバータイマー更新
		gameOverTimer_ -= TimeManager::GetInstance()->GetDeltaTime();
		// タイマーが0以下になったら
		if (gameOverTimer_ <= 0.0f) {
			// シーン切り替え
			SceneManager::GetInstance()->ChangeScene(SceneName::GameOver);
		}
		return;
	}

	// スタート演出中
	if (!startSequence_->IsFinished()) {
		startSequence_->Update(TimeManager::GetInstance()->GetDeltaTime());
		return;
	}

	// ポーズ入力の更新
	pauseController_->Update();
	
	// ポーズ中ならゲーム更新を止める
	if (pauseController_->IsPaused()) return;

	// クリア条件
	if (railFollowSystem_->IsFinished()) {
		// ゲームクリアフラグON
		isGameClear_ = true;
		// クリアタイマーセット
		gameClearTimer_ = kGameClearDuration;
		return;
	}

	// ゲームオーバー条件
	if (player_->IsDead()) {
		// ゲームオーバーフラグON
		isGameOver_ = true;
		// ゲームオーバータイマーセット
		gameOverTimer_ = kGameOverDuration;
		return;
	}

	// 3Dオブジェクトの更新処理
	// プレイヤーの更新処理
	player_->Update(TimeManager::GetInstance()->GetDeltaTime());

	railFollowSystem_->SetPlayerPosition(player_->GetWorldPosition());
	railFollowSystem_->SetPlayerSpeed(player_->GetMoveSpeed());
	railFollowSystem_->SetInput(player_->GetMoveInput());

	// レール追従システムの更新
	railFollowSystem_->Update();

	Vector3 playerRotate = MyTools::Lerp(player_->GetObject3D()->GetRotate(), railFollowSystem_->GetPlayerRotate(), 0.1f);

	player_->SetWorldPosition(railFollowSystem_->GetPlayerPosition());
	player_->GetObject3D()->SetRotate(playerRotate);

	// 敵の更新処理
	for(std::unique_ptr<Enemy>& enemy : enemies_){
		enemy->Update(TimeManager::GetInstance()->GetDeltaTime());
	}

	// 天球の更新
	skydome_->GetObject3D()->SetTranslate(player_->GetWorldPosition());
	skydome_->Update();

	if (isAccelerationField_) {
		for (pair<const string, unique_ptr<ParticleManager::ParticleGroup>>& pair : ParticleManager::GetInstance()->GetParticleGroups()) {
			ParticleManager::ParticleGroup& group = *pair.second;
			int index = 0;
			for (list<Particle>::iterator it = group.particles.begin(); it != group.particles.end();) {
				Particle& particle = *it;

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
	escapeUI_->Update();

#ifdef _DEBUG
	// デバッグラインの追加
	AddDebugLines();
#endif // _DEBUG
}

// 描画
void GameScene::Draw() {
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
	for(unique_ptr<Enemy>& enemy : enemies_){
		enemy->Draw();
	}

	// プレイヤーの描画
	player_->Draw();

#pragma endregion 3Dオブジェクト

#pragma region パーティクル

	// パーティクルの描画準備。パーティクルの描画に共通グラフィックスコマンドを積む
	ParticleManager::GetInstance()->Draw();

#pragma endregion パーティクル

#pragma region デバッグライン
#ifdef _DEBUG
	DebugLineManager::GetInstance()->DrawAll();
#endif // _DEBUG
#pragma endregion デバッグライン

#pragma region スプライト

	// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	TextureManager::GetInstance()->SetCommonScreen();

	// 全てのSprite個々の描画
	escapeUI_->Draw();

#pragma endregion スプライト

#pragma region ポーズ関連
	// ポーズ中はUIを最前面に描画
	if (pauseController_->IsPaused()) {
		pauseController_->Draw();
	}
#pragma endregion
}

#ifdef _DEBUG
// デバッグ更新
void GameScene::DebugUpdate() {
	if (railCamera_->IsDebugMode()) {
		Matrix4x4 wvp = CameraManager::GetInstance()->GetCamera()->GetViewProjectionMatrix();

		DebugLineBase::GetInstance()->UpdateMatrix(wvp);
		DebugLineBase::GetInstance()->UpdateMaterial({ 0.0f, 1.0f, 0.0f, 1.0f });
	}

	DebugDraw();
}

// デバッグ描画
void GameScene::DebugDraw() {
	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::SetNextWindowPos(kDebugWindowPosSettings, ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(kDebugWindowSizeSettings, ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Settings");
	// Camera
	CameraManager::GetInstance()->DebugDraw();
	// FollowCamera
	followCamera_->DebugDraw();
	// RailCamera
	railCamera_->DebugDraw();
	if(railCamera_->IsDebugMode()){
		DebugLineBase::GetInstance()->DebugDraw();
		DebugLineManager::GetInstance()->DebugDraw();
	}

	// Lighting
	LightManager::GetInstance()->DebugDraw();

	// Skydome
	skydome_->DebugDraw();

	// プレイヤー
	player_->DebugDraw();

	// 敵
	for(unique_ptr<Enemy>& enemy : enemies_){
		enemy->DebugDraw();
	}

	// パーティクル
	ParticleManager::GetInstance()->ImGui();

	ImGui::End();
}

/// デバッグライン追加
void GameScene::AddDebugLines() {
	DebugLineManager::GetInstance()->DrawSpline(DebugLineCategory::rail, kRailPoints, 32,{ 0.0f, 1.0f, 0.0f, 1.0f });
}
#endif // _DEBUG

// JSONファイルの読み込み
void GameScene::LoadJsonFile([[maybe_unused]] const string& filePath) {
	// レベルデータの読み込み
	unique_ptr<JsonLevelData> levelData = jsonLoader_->LoadFile(filePath);
	
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