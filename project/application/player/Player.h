#pragma once
#include "Collider.h"
#include <list>
#include "Object3d.h"
#include "TextureManager.h"

// プレイヤー
class Player : public Collider
{
public:	// メンバ関数
	Player();
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	void Initialize(MyBase::Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

#ifdef _DEBUG
	/// <summary>
	/// デバック描画
	/// </summary>
	void DebugDraw();
#endif // _DEBUG


	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attaack();

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision() override;

public:	// getter
	// 
	Vector3 GetWorldPosition() override { return object_->GetTranslate(); };

public:	// setter


private:	// メンバ変数
	// モデル
	std::unique_ptr<Object3d> object_ = nullptr;
	
	// ステータス
	int hp_;
	bool isDead_ = false;
	
	// プレイヤーの移動速度
	const float kmoveSpeed_ = 0.2f;

};

