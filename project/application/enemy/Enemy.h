#pragma once
#include "BaseObject.h"
#include <list>
#include "TextureManager.h"
#include "ParticleEmitter.h"

class Enemy : public BaseObject
{
public:	// メンバ関数
	Enemy();
	~Enemy();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
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

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

public:	// getter
	// 
	Vector3 GetWorldPosition() override { return BaseObject::GetWorldPosition(); };

public:	// setter


private:	/// メンバ変数
	// パーティクルエミッター
	std::unique_ptr<ParticleEmitter> particleEmitter_ = nullptr;

	// ステータス
	int hp_;
	bool isDead_ = false;

};

