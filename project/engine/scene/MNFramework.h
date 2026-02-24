#pragma once
#include "D3DResourceLeakChecker.h"
#include "WindowsAPI.h"
#include "DirectXBase.h"
#include "Input.h"
#include "SrvManager.h"
#include "OffScreen.h"
#include "CollisionManager.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "ModelManager.h"
#include "AudioManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"
#ifdef _DEBUG
#include "ImGuiManager.h"
#include "DebugLineBase.h"
#endif // _DEBUG

/// <summary>
/// ゲームフレームワーク基底クラス(MNFramework)
/// ゲーム全体のライフサイクル管理と各種マネージャの統括を行う抽象クラスです。
/// </summary>
class MNFramework
{
public:	// メンバ関数
	virtual ~MNFramework() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(const wchar_t* windowTitle);

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了チェック
	/// </summary>
	/// <returns>終了要求が来たらtrueを返す</returns>
	virtual bool IsEndRequest() { return endRequest_; }

	/// <summary>
	/// 実行
	/// </summary>
	void Run(const wchar_t* windowTitle);

protected:	// メンバ変数
	D3DResourceLeakChecker leakCheck;

	// ゲーム終了フラグ
	bool endRequest_ = false;

	// ポインタ
	// windowsAPI
	std::unique_ptr<WindowsAPI> winApi_ = nullptr;
	// DirectXBase
	DirectXBase* dxBase_ = nullptr;
	// 入力
	Input* input_ = nullptr;
	// SRVマネージャ
	std::unique_ptr<SrvManager> srvManager_ = nullptr;
#ifdef _DEBUG
	// ImGuiManagerの宣言
	std::unique_ptr<ImGuiManager> imGuiManager_ = nullptr;
	// デバッグライン
	std::unique_ptr<DebugLineBase> debugLine_ = nullptr;
#endif // _DEBUG
	// オフスクリーン
	std::unique_ptr<OffScreen> offScreen_ = nullptr;
	// コリジョンマネージャー
	CollisionManager* collisionManager_ = nullptr;
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
	// タイムマネージャ
	TimeManager* timeManager_ = nullptr;
	// シーンマネージャ
	SceneManager* sceneManager_ = nullptr;
	// シーンファクトリー
	std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;
};