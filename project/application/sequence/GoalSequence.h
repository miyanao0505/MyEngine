#pragma once

#include <memory>
#include "MyBase.h"
#include "BaseObject.h"

// 前方宣言（依存を最小限に）
class Camera;
class Player;

/// <summary>
/// ゴール到達後の演出制御クラス
/// カメラ回転 → クリアロゴ拡大 → 終了
/// </summary>
class GoalSequence
{
private:
#pragma region 定数
	static constexpr float kRotateDuration = 2.0f;

	static constexpr float kLogoScaleDuration = 1.5f;
	static constexpr float kLogoMaxScale = 4.0f;
#pragma endregion

public:
	/// <summary>
	/// 演出ステート
	/// </summary>
	enum class State {
		StartSet,     // 初期化
		CameraRotate,   // カメラ左回転
		LogoScale,      // ロゴ拡大
		End,            // 演出終了
	};

public: // メンバ関数
	GoalSequence();
	~GoalSequence() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player">プレイヤーオブジェクトへのポインタ</param>
	/// <param name="camera">カメラオブジェクトへのポインタ</param>
	/// <param name="cameraOffset">カメラオフセット</param>
	void Initialize(Player* player, Camera* camera, const MyBase::Vector3& cameraOffset);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaTime">経過時間（秒）</param>
	void Update(float deltaTime);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
#pragma region スマート更新
	void UpdateStartSet();
	void UpdateCameraRotate(float deltaTime);
	void UpdateLogoScale(float deltaTime);
#pragma endregion

#pragma region 補助処理
	void UpdateCameraPosition(float angleDeg);
	float EaseInOut(float t);
#pragma endregion   

public: // getter
	/// <summary>
	/// 演出終了判定
	/// </summary>
	bool IsFinished() const { return isFinished_; }

public: // setter

private:
	Player* player_ = nullptr;
	Camera* camera_ = nullptr;
	std::unique_ptr<BaseObject> clearLogo_ = nullptr;

	State state_ = State::StartSet;
	float stateTimer_ = 0.0f;
	bool isFinished_ = false;

	float currentAngle_ = 0.0f;
	MyBase::Vector3 cameraOffset_;
	float cameraDistance_;
	float cameraHight_;
};