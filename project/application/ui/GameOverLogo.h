#pragma once
#include "BaseObject.h"

/// <summary>
/// ゲームオーバーシーンで表示される「GAME OVER」ロゴおよび関連オブジェクトを
/// 管理・描画するクラスです。
/// </summary>
class GameOverLogo
{
private:
#pragma region 定数
	static const MyBase::Transform kCharTransform;
	static const MyBase::Transform kButtonTransform;
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

public:	// getter

public:	// setter

private:	// メンバ変数
	// 3Dオブジェクト
	std::unique_ptr<BaseObject> gameOverChar_ = nullptr;
	std::unique_ptr<BaseObject> transitionButton_ = nullptr;
};

