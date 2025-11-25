#pragma once
#include "BaseObject.h"
#include "TextureManager.h"

/// <summary>
/// シーン全体を包み込む**天球（スカイドーム）オブジェクト**を管理するクラスです。
/// 主に背景として使用され、カメラの位置やシーンの状態に関わらず、
/// 常にプレイヤーの視界を覆うように描画されます。
/// </summary>
class Skydome : public BaseObject
{
public:	// メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Skydome();

	/// <summary>
	/// デストラクタ
	/// </summary>
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
	/// <summary>
	/// オブジェクトのワールド座標を取得
	/// </summary>
	/// <returns>オブジェクトのワールド座標(Vector3)</returns>
	MyBase::Vector3 GetWorldPosition() override { return BaseObject::GetWorldPosition(); };

private:	// メンバ変数
	
};

