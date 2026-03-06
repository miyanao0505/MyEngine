#include "MNFramework.h"
#include "SceneFactory.h"

// 初期化
void MNFramework::Initialize(const wchar_t* windowTitle)
{
	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

#pragma region ゲームウィンドウ作成
	// WindowsAPIの初期化
	winApi_ = std::make_unique<WindowsAPI>();
	winApi_->Initialize(windowTitle);
#pragma endregion ゲームウィンドウ作成

#pragma region DirectX初期化
	// DirectXBaseの初期化
	dxBase_ = DirectXBase::GetInstance();
	dxBase_->Initialize(winApi_.get());
#pragma endregion DirectX初期化

#pragma region 汎用機能初期化
	// 入力の初期化
	input_ = Input::GetInstance();
	input_->Initialize(winApi_.get());
#pragma endregion 汎用機能初期化

#pragma region 基盤システム初期化
	// SRVマネージャーの初期化
	srvManager_ = std::make_unique<SrvManager>();
	srvManager_->Initialize();

#ifdef _DEBUG
	// ImGuiManagerの初期化
	imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(winApi_.get(), srvManager_.get());

	// デバッグラインの初期化
	DebugLineBase::GetInstance()->Initialize(dxBase_);
#endif // _DEBUG

	// オフスクリーンの作成
	offScreen_ = std::make_unique<OffScreen>();
	offScreen_->Initialize();
	dxBase_->CreateOffScreenSRV(srvManager_.get());

	// コリジョンマネージャの初期化
	collisionManager_ = CollisionManager::GetInstance();
	collisionManager_->Clear();
#pragma endregion 基盤システム初期化

	// カメラマネージャの初期化
	cameraManager_ = CameraManager::GetInstance();
	cameraManager_->Initialize();

	// ライトマネージャの初期化
	lightManager_ = LightManager::GetInstance();
	lightManager_->Initialize();

	// テクスチャマネージャの初期化
	textureManager_ = TextureManager::GetInstance();
	textureManager_->Initialize(srvManager_.get());

	// パーティクルマネージャの初期化
	particleManager_ = ParticleManager::GetInstance();
	particleManager_->Initialize(srvManager_.get());

	// モデルマネージャの初期化
	modelManager_ = ModelManager::GetInstance();
	modelManager_->Initialize();

	// オーディオマネージャの初期化
	audioManager_ = AudioManager::GetInstance();
	audioManager_->Initialize();

	// タイムマネージャの初期化
	timeManager_ = TimeManager::GetInstance();
	timeManager_->Initialize();

	// シーンマネージャの生成
	sceneFactory_ = std::make_unique<SceneFactory>();
	sceneManager_ = SceneManager::GetInstance();
	sceneManager_->SetSceneFactory(sceneFactory_.get());
#pragma endregion マネージャ初期化
}

// 終了
void MNFramework::Finalize()
{
	// 終了
	sceneManager_->Finalize();
	timeManager_->Finalize();
	audioManager_->Finalize();
	modelManager_->Finalize();
	particleManager_->Finalize();
	textureManager_->Finalize();
	lightManager_->Finalize();
	cameraManager_->Finalize();
	collisionManager_->Finalize();
#ifdef _DEBUG
	DebugLineBase::GetInstance()->Finalize();
	imGuiManager_->Finalize();
#endif // _DEBUG
	input_->Finalize();
	dxBase_->Finalize();
	winApi_->Finalize();
}

// 毎フレーム更新
void MNFramework::Update()
{
	// ゲーム処理

	// 入力の更新処理
	Input::GetInstance()->Update();

	// 終了
	if (winApi_->ProcessMessage()) {
		endRequest_ = true;
		return;
	}

	// ImGuiにここからフレームが始まる旨を告げる
#ifdef _DEBUG
	imGuiManager_->Begin();
#endif // _DEBUG
	// シーンマネージャの更新処理
	sceneManager_->Update();
	// コリジョンマネージャーの更新処理
	collisionManager_->Update();
	
	// ImGuiの内部コマンドを生成する
#ifdef _DEBUG
	imGuiManager_->End();
#endif // _DEBUG
}

void MNFramework::Run(const wchar_t* windowTitle)
{
	// ゲームの初期化
	Initialize(windowTitle);

	while (true)	// ゲームループ
	{
		// 毎フレーム更新
		Update();
		// 終了リクエストが来たら抜ける
		if (IsEndRequest()) {
			break;
		}
		// 描画
		Draw();
	}
	// ゲーム終了
	Finalize();
}
