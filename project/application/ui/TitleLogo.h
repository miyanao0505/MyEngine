#pragma once
#include "BaseObject.h"
#include "MyBase.h"

/// <summary>
/// タイトルシーンにおけるタイトルロゴおよびスタートボタンの
/// 表示とアニメーションを管理するクラスです。
/// </summary>
class TitleLogo
{
private:
#pragma region 定数
	static const MyBase::Transform kCharFirstTransform;
	static const MyBase::Transform kCharSecondTransform;
	static const MyBase::Transform kCharThirdTransform;
	static const MyBase::Transform kButtonTransform;

	static const MyBase::Vector3 kMoveVector;		// ロゴ移動ベクトル
	static const float kMoveDistance;				// ロゴ移動距離
	static const float kMoveSpeed;					// ロゴ移動速度
	static constexpr float kReverse = -1.0f;		// ロゴ移動反転係数
	static constexpr float kMaxMoveDistance = 1.50f;	// ロゴの最大移動距離
#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ロゴの移動処理
	/// </summary>
	void Move(float deltaTime);

public:	// getter

public:	// setter

private:	// メンバ変数
	// 3Dオブジェクト
	std::unique_ptr<BaseObject> titleCharFirst_ = nullptr;
	std::unique_ptr<BaseObject> titleCharSecond_ = nullptr;
	std::unique_ptr<BaseObject> titleCharThird_ = nullptr;
	std::unique_ptr<BaseObject> startButton_ = nullptr;

	MyBase::Vector3 moveVector_{ 0.0f, 0.0f, 0.0f };
	float moveDistance_;
};