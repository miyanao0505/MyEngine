#pragma once
#include "BaseObject.h"
#include "TextureManager.h"

class Skydome : public BaseObject
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
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	
#ifdef _DEBUG
	/// <summary>
	/// デバック描画
	/// </summary>
	void DebugDraw() override;
#endif // _DEBUG

public:	// getter
	// 
	Vector3 GetWorldPosition() override { return BaseObject::GetWorldPosition(); };

private:	// メンバ変数
	
};

