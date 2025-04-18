#pragma once
#include "D3DResourceLeakChecker.h"
#include "WindowsAPI.h"
#include "DirectXBase.h"
#include "Input.h"
#include "SrvManager.h"
#include "ImGuiManager.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "ModelManager.h"
#include "AudioManager.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"

// ゲーム全体
class MNFramework
{
public:	// メンバ関数
	virtual ~MNFramework() = default;

	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 毎フレーム更新
	virtual void Update();

	// 描画
	virtual void Draw() = 0;

	// 終了チェック
	virtual bool IsEndRequest() { return endRequest_; }

	// 実行
	void Run();

protected:	// メンバ変数
	D3DResourceLeakChecker leakCheck;

	// ゲーム終了フラグ
	bool endRequest_ = false;

	// ポインタ
	// windowsAPI
	std::unique_ptr<WindowsAPI> winApi_ = nullptr;
	// DirectXBase
	std::unique_ptr<DirectXBase> dxBase_ = nullptr;
	// 入力
	Input* input_ = nullptr;
	// SRVマネージャ
	std::unique_ptr<SrvManager> srvManager_ = nullptr;
#ifdef _DEBUG
	// ImGuiManagerの宣言
	std::unique_ptr<ImGuiManager> imGuiManager_ = nullptr;
#endif // _DEBUG
	// カメラマネージャ
	CameraManager* cameraManager_ = nullptr;
	// ライトマネージャ
	LightManager* lightManager_ = nullptr;
	// テクスチャマネージャ
	TextureManager* textureManager_ = nullptr;
	// パーティクルマネージャ
	ParticleManager* particleManager_ = nullptr;
	// モデルマネージャ
	ModelManager* modelManager_ = nullptr;
	// オーディオマネージャ
	AudioManager* audioManager_ = nullptr;
	// シーンマネージャ
	SceneManager* sceneManager_ = nullptr;
	// シーンファクトリー
	std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;
};

