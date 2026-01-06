#pragma once
#include "BaseObject.h"
#include "MyBase.h"

/// <summary>
/// クリアシーンで表示される「CLEAR」ロゴおよび関連オブジェクトを
/// 管理・描画するクラスです。
/// </summary>
class ClearLogo
{
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
	void Update(float deltaTime);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ロゴの移動処理
	/// </summary>
	void Move();

public:	// getter

public:	// setter

private:	// メンバ変数
	// 3Dオブジェクト
	std::unique_ptr<BaseObject> clearChar_ = nullptr;
	std::unique_ptr<BaseObject> transitionButton_ = nullptr;

	MyBase::Vector3 moveVector_{ 0.0f, 0.0f, 0.0f };
	float moveDistance_ = 0.0f;
	const float kMoveSpeed = 0.05f;
};