#pragma once
#include "BaseObject.h"
#include <list>
#include "EnemyBaseState.h"
#include "TextureManager.h"
#include "ParticleEmitter.h"

/// 前方宣言
class Player;

/// <summary>
/// Enemyクラス
/// 自機(Player)と対峙する敵キャラクターを表すクラス。
/// </summary>
class Enemy : public BaseObject
{
public:	// メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy();

	/// <summary>
	/// デストラクタ
	/// </summary>
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

	/// <summary>
	/// Updateのステートチェンジ
	/// </summary>
	/// <param name="state">新しいステート</param>
	void ChangeState(std::unique_ptr<EnemyBaseState> state);

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
	/// <summary>
	/// プレイヤーのポインタを取得
	/// </summary>
	/// <returns>プレイヤーのポインタ</returns>
	Player* GetPlayer() { return player_; }

	/// <summary>
	/// 敵が死亡しているかを取得
	/// </summary>
	/// <returns>死亡しているか</returns>
	bool IsDead() { return isDead_; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	MyBase::Vector3 GetWorldPosition() override { return BaseObject::GetWorldPosition(); };

public:	// setter
	/// <summary>
	/// プレイヤーのポインタを設定
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	void SetPlayer(Player* player) { player_ = player; }

private:	/// メンバ変数
	// パーティクルエミッター
	std::unique_ptr<ParticleEmitter> particleEmitter_ = nullptr;

	// ステータス
	int hp_;
	bool isDead_ = false;
	std::unique_ptr<EnemyBaseState> state_;

	// プレイヤー
	Player* player_;
};

