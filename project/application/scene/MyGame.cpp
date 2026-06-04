#include "MyGame.h"
#include"SceneFactory.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#pragma comment(lib, "dxcompiler.lib")
#ifdef _DEBUG
#include "DebugLineManager.h"
#endif // _DEBUG

// 初期化
void MyGame::Initialize(const wchar_t* windowTitle)
{
	// 基底クラスの初期化処理
	MNFramework::Initialize(windowTitle);

	// シーンマネージャに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene(SceneName::Title);

#pragma region カメラ
	CameraManager::GetInstance()->AddCamera("default");
	CameraManager::GetInstance()->SetCamera("default");
	CameraManager::GetInstance()->GetCamera()->SetTranslate(kDefaultCameraTranslate);
	CameraManager::GetInstance()->GetCamera()->SetRotate(kDefaultCameraRotate);
	CameraManager::GetInstance()->AddCamera("sub");
	CameraManager::GetInstance()->SetCamera("sub");
	CameraManager::GetInstance()->GetCamera()->SetTranslate(kSubCameraTranslate);
	CameraManager::GetInstance()->GetCamera()->SetRotate(kSubCameraRotate);

	CameraManager::GetInstance()->SetCamera("default");
#pragma endregion
}

// 終了
void MyGame::Finalize()
{
	// COMの終了処理
	CoUninitialize();

	// 基底クラスの終了処理
	MNFramework::Finalize();
}

// 毎フレーム更新
void MyGame::Update()
{
	// 基底クラスの更新処理
	MNFramework::Update();
}

// 描画
void MyGame::Draw()
{
	// DirectXの描画前処理。全ての描画に共通のグラフィックスコマンドを積む
	dxBase_->PreRenderTexture();
	
	srvManager_->PreDraw();

	// カメラの更新
	cameraManager_->GetCamera()->Update();

	// シーンマネージャの描画
	sceneManager_->Draw();

	// DirectXの描画前処理。全ての描画に共通のグラフィックスコマンドを積む
	dxBase_->PreDraw();
	offScreen_->Draw();
	// 実際のcommandListのImGuiの描画コマンドを積む
#ifdef _DEBUG
	imGuiManager_->Draw();
#endif // _DEBUG

	// 描画後処理
	dxBase_->PostDraw();

}
