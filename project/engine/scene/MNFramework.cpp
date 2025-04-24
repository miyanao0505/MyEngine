#include "MNFramework.h"
#include "SceneFactory.h"

// 初期化
void MNFramework::Initialize()
{
	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

#pragma region ゲームウィンドウ作成
	// WindowsAPIの初期化
	winApi_.reset(new WindowsAPI());
	winApi_->Initialize(L"GE3");
#pragma endregion ゲームウィンドウ作成

#pragma region DirectX初期化
	// DirectXBaseの初期化
	dxBase_.reset(new DirectXBase());
	dxBase_->Initialize(winApi_.get());
#pragma endregion DirectX初期化

#pragma region 汎用機能初期化
	// 入力の初期化
	input_ = Input::GetInstance();
	input_->Initialize(winApi_.get());
#pragma endregion 汎用機能初期化

#pragma region 基盤システム初期化
	// SRVマネージャーの初期化
	srvManager_.reset(new SrvManager());
	srvManager_->Initialize(dxBase_.get());

	// オフスクリーンの作成
	dxBase_->CreateOffScreenSRV(srvManager_.get());
#pragma endregion 基盤システム初期化

#pragma region マネージャ初期化
#ifdef _DEBUG
#pragma region ImGui初期化
	// ImGuiManagerの初期化
	imGuiManager_.reset(new ImGuiManager());
	imGuiManager_->Initialize(winApi_.get(), dxBase_.get(), srvManager_.get());
#pragma endregion
#endif // _DEBUG

	// カメラマネージャの初期化
	cameraManager_ = CameraManager::GetInstance();
	cameraManager_->Initialize();
	cameraManager_->SetCamera("default");
	cameraManager_->FindCamera("default");
	cameraManager_->GetCamera()->SetRotate({ 0.3f, 0.0f, 0.0f });
	cameraManager_->GetCamera()->SetTranslate({ 0.0f, 4.0f, -10.0f });
	cameraManager_->SetCamera("sub");
	cameraManager_->FindCamera("sub");
	cameraManager_->GetCamera()->SetRotate({ 0.3f, 3.1f, 0.0f });
	cameraManager_->GetCamera()->SetTranslate({ 0.0f, 4.0f, 10.0f });

	cameraManager_->FindCamera("default");

	// ライトマネージャの初期化
	lightManager_ = LightManager::GetInstance();
	lightManager_->Initialize(dxBase_.get());

	// テクスチャマネージャの初期化
	textureManager_ = TextureManager::GetInstance();
	textureManager_->Initialize(dxBase_.get(), srvManager_.get());

	// パーティクルマネージャの初期化
	particleManager_ = ParticleManager::GetInstance();
	particleManager_->Initialize(dxBase_.get(), srvManager_.get());

	// モデルマネージャの初期化
	modelManager_ = ModelManager::GetInstance();
	modelManager_->Initialize(dxBase_.get());

	// オーディオマネージャの初期化
	audioManager_ = AudioManager::GetInstance();
	audioManager_->Initialize();

	// シーンマネージャの生成
	sceneFactory_.reset(new SceneFactory());
	sceneManager_ = SceneManager::GetInstance();
	sceneManager_->SetSceneFactory(sceneFactory_.get());
#pragma endregion マネージャ初期化
}

// 終了
void MNFramework::Finalize()
{
	// 終了
	sceneManager_->Finalize();
	audioManager_->Finalize();
	modelManager_->Finalize();
	particleManager_->Finalize();
	textureManager_->Finalize();
	lightManager_->Finalize();
	cameraManager_->Finalize();
#ifdef _DEBUG
	imGuiManager_->Finalize();
#endif // _DEBUG
	input_->Finalize();
	winApi_->Finalize();
}

// 毎フレーム更新
void MNFramework::Update()
{
	// ゲーム処理

	// 入力の更新処理
	Input::GetInstance()->Update();

	// 終了
	if (winApi_->ProcessMessage() || input_->TriggerKey(DIK_ESCAPE)) {
		endRequest_ = true;
		return;
	}

	// ImGuiにここからフレームが始まる旨を告げる
#ifdef _DEBUG
	imGuiManager_->Begin();
#endif // _DEBUG

	// シーンマネージャの更新処理
	sceneManager_->Update();
	
	// ImGuiの内部コマンドを生成する
#ifdef _DEBUG
	imGuiManager_->End();
#endif // _DEBUG
}

void MNFramework::Run()
{
	// ゲームの初期化
	Initialize();

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
