#pragma once
#include "Object3d.h"
#include "TextureManager.h"

class Skydome
{
public:	// メンバ関数
	Skydome();
	~Skydome();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(MyBase::Vector3 position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
private:	// メンバ変数
	std::unique_ptr<Object3d> object_;
	std::string filePath_;
	
};

