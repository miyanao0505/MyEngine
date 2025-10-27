#pragma once
#include "BaseObject.h"
#include "MyBase.h"

class GameOverLogo
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
};

