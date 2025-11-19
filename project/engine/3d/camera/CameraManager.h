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
	// シングルトンインスタンスの取得
	static CameraManager* GetInstance();
	// 終了
	void Finalize();

	// 初期化
	void Initialize();

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="cameraName">カメラの名前</param>
	void SetCamera(const std::string& cameraName);

	/// <summary>
	/// カメラの検索
	/// </summary>
	/// <param name="cameraName">カメラの名前</param>
	void FindCamera(const std::string& cameraName);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaTime"></param>
	void Update(float deltaTime);

	/// <summary>
	/// シェイクを開始
	/// </summary>
	/// <param name="amplitude">平行移動の最大振幅(ワールド単位)</param>
	/// <param name="duration">継続時間(秒)</param>
	/// <param name="frequency">揺れの周波数(Hz) - 実装では時間に掛ける係数</param>
	/// <param name="rotationAmplitude">回転の最大振幅(ラジアン)</param>
	void StartShake(float amplitude, float duration, float frequency = 20.0f, float rotationAmplitude = 0.02f);

	/// <summary>
	/// 即時停止(オリジナルに復帰)
	/// </summary>
	void StopShake();

#ifdef _DEBUG
	// デバック用の描画
	void DebugDraw();
#endif // _DEBUG

public:	// getter
	std::vector<std::string> GetAllName();

private:	// シングルトン
	static CameraManager* instance;

	CameraManager() = default;
	~CameraManager() = default;
	CameraManager(CameraManager&) = default;
	CameraManager& operator=(CameraManager&) = delete;

public:	// メンバ関数
	/// <summary>
	/// カメラデータの取得
	/// </summary>
	/// <returns>カメラ</returns>
	Camera* GetCamera() { return camera_; }

private:	// メンバ変数
	// カメラデータ
	std::map<std::string, std::unique_ptr<Camera>> cameras_;

	// 現在指定しているカメラデータ
	Camera* camera_ = nullptr;
	std::string cameraName_ = "";

	// シェイク状態
	ShakeState shakeState_;
};

