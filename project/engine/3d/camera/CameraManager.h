#pragma once
#include <map>
#include <memory>
#include <string>
#include <random>
#include "Camera.h"

/// <summary>
/// 複数のカメラを管理し、現在アクティブなカメラの制御および
/// カメラシェイク演出を行うマネージャークラス。
/// </summary>
class CameraManager
{
private:	// 構造体
	struct ShakeState {
		bool active{ false };
		float duration{ 0.0f };
		float timer{ 0.0f };
		float amplitude{ 0.0f };
		float frequency{ 0.0f };
		float rotationAmplitude{ 0.0f };
		MyBase::Vector3 originalTranslate{};
		// RNG
		std::mt19937 rng;
		std::uniform_real_distribution<float> dist{ -1.0f, 1.0f };
		ShakeState() : rng(std::random_device{}()) {}
	};

public:	// メンバ関数
	/// <summary>
	/// CameraManager のシングルトンインスタンスを取得
	/// </summary>
	/// <returns>CameraManager の唯一のインスタンス</returns>
	static CameraManager* GetInstance();

	/// <summary>
	/// CameraManager を終了(解放)
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaTime">前フレームからの経過時間(秒)</param>
	void Update(float deltaTime);

	/// <summary>
	/// 新しいカメラを追加し、cameras_ に登録する
	/// 既に同名のカメラが存在する場合は何もしない
	/// </summary>
	/// <param name="cameraName">登録するカメラ名</param>
	void AddCamera(const std::string& cameraName);

	/// <summary>
	/// カメラシェイクを開始
	/// </summary>
	/// <param name="amplitude">平行移動の最大振幅(ワールド単位)</param>
	/// <param name="duration">継続時間(秒)</param>
	/// <param name="frequency">揺れの周波数(Hz、実装では時間に掛ける係数)</param>
	/// <param name="rotationAmplitude">回転の最大振幅(ラジアン)</param>
	void StartShake(float amplitude, float duration, float frequency = 20.0f, float rotationAmplitude = 0.02f);

	/// <summary>
	/// カメラシェイクを即時停止し、オリジナルの状態に復帰
	/// </summary>
	void StopShake();

#ifdef _DEBUG
	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw();
#endif // _DEBUG

public:	// getter
	/// <summary>
	/// 登録されているすべてのカメラ名を取得
	/// </summary>
	/// <returns>カメラ名のリスト</returns>
	std::vector<std::string> GetAllNames();

	/// <summary>
	/// カメラデータの取得
	/// </summary>
	/// <returns>カメラ</returns>
	Camera* GetCamera() { return activeCamera_; }

public:	// setter
	/// <summary>
	/// 既存のカメラの中からアクティブカメラを選択する
	/// </summary>
	/// <param name="cameraName">切り替え先のカメラ名</param>
	void SetCamera(const std::string& cameraName);

private:	// シングルトン
	static CameraManager* sInstance;

	CameraManager() = default;
	~CameraManager() = default;
	CameraManager(CameraManager&) = delete;
	CameraManager& operator=(CameraManager&) = delete;

private:	// メンバ変数
	// カメラデータ
	std::map<std::string, std::unique_ptr<Camera>> cameras_;

	// 現在指定しているカメラデータ
	Camera* activeCamera_ = nullptr;
	std::string activeCameraName_ = "";

	// シェイク状態
	ShakeState shakeState_;
};