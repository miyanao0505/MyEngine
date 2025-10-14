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
	/// <param name="filePath">テクスチャファイルパス</param>
	/// <param name="position">初期位置</param>
	/// <param name="scale">初期スケール</param>
	void Initialize(const std::string& filePath, MyBase::Vector3 position, MyBase::Vector3 scale);
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
	MyBase::Vector3 GetWorldPosition() override { return BaseObject::GetWorldPosition(); };

private:	// メンバ変数
	
};

