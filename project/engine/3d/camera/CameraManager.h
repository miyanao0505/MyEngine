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
	/// 名前でカメラを検索
	/// </summary>
	/// <param name="cameraName">検索するカメラの名前</param>
	void FindCamera(const std::string& cameraName);

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
	std::vector<std::string> GetAllName();

	/// <summary>
	/// カメラデータの取得
	/// </summary>
	/// <returns>カメラ</returns>
	Camera* GetCamera() { return camera_; }

public:	// setter
	/// <summary>
	/// 現在操作するカメラを設定
	/// </summary>
	/// <param name="cameraName">設定するカメラの名前</param>
	void SetCamera(const std::string& cameraName);

private:	// シングルトン
	static CameraManager* instance;

	CameraManager() = default;
	~CameraManager() = default;
	CameraManager(CameraManager&) = default;
	CameraManager& operator=(CameraManager&) = delete;

private:	// メンバ変数
	// カメラデータ
	std::map<std::string, std::unique_ptr<Camera>> cameras_;

	// 現在指定しているカメラデータ
	Camera* camera_ = nullptr;
	std::string cameraName_ = "";

	// シェイク状態
	ShakeState shakeState_;
};

