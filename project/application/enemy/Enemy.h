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
private:
#pragma region 定数
	static constexpr float kDamageReactionDuration = 0.1f;	// ダメージリアクション時間
	static constexpr float kDeadReactionDuration = 1.0f;	// 死亡リアクション時間

	
#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy() = default;

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
	void Update(float deltaTime) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// Updateのステートチェンジ
	/// </summary>
	/// <param name="state">新しいステート</param>
	void ChangeState(std::unique_ptr<EnemyBaseState> state);

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="damage">ダメージ値</param>
	void Damage(int damage);

	/// <summary>
	/// ダメージリアクションの開始
	/// </summary>
	void DamageReactionStart();

	/// <summary>
	/// ダメージリアクションの更新
	/// </summary>
	void DamageReactionUpdate();

	/// <summary>
	/// 死亡リアクション
	/// </summary>
	void DeadReaction();

#ifdef _DEBUG
	/// <summary>
	/// デバッグ描画
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
	Player* GetPlayer() const { return player_; }

	/// <summary>
	/// 敵が死亡しているかを取得
	/// </summary>
	/// <returns>死亡しているか</returns>
	bool IsDead() const { return isDead_; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	MyBase::Vector3 GetWorldPosition() const override { return BaseObject::GetWorldPosition(); };

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

	// 演出系
	float damageReactionTimer_ = 0.0f;
	float deadReactionTimer_ = 0.0f;

	// プレイヤー
	Player* player_;
};