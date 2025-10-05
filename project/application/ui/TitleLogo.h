#pragma once
#include "BaseObject.h"
#include "MyBase.h"

class TitleLogo
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
	std::unique_ptr<BaseObject> titleCharFirst_ = nullptr;
	std::unique_ptr<BaseObject> titleCharSecond_ = nullptr;
	std::unique_ptr<BaseObject> titleCharThird_ = nullptr;

};

