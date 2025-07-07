#pragma once
#include "Collider.h"
#include <list>
#include "Object3d.h"
#include "TextureManager.h"
#include "ParticleEmitter.h"

class Enemy : public Collider
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
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

public:	// getter
	// 
	Vector3 GetWorldPosition() override { return object_->GetTranslate(); };

public:	// setter


private:	/// メンバ変数
	// モデル
	std::unique_ptr<Object3d> object_ = nullptr;

	// パーティクルエミッター
	std::unique_ptr<ParticleEmitter> particleEmitter_ = nullptr;

	// ステータス
	int hp_;
	bool isDead_ = false;

};

