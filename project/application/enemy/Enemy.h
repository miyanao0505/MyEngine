#pragma once
#include "BaseObject.h"
#include <list>
#include "EnemyBaseState.h"
#include "TextureManager.h"
#include "ParticleEmitter.h"
#include "EnemyBullet.h"

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
	static constexpr float kAttackCoolTime = 1.5f;			// 攻撃クールタイム

	static const int kMaxBulletCount;				// 最大弾数
	static const float kBulletDrawDistance;			// 弾の描画距離
	static const MyBase::Vector3 kBulletSpawnOffset;	// 弾の発射位置オフセット

	static const MyBase::Vector3 kInitialPosition;	// 敵の初期位置
	static const MyBase::Vector3 kInitialScale;		// 敵の初期スケール
	static const MyBase::Vector3 kInitialRotation;	// 敵の初期回転
	static const MyBase::Vector4 kInitialColor;		// 敵の初期色

	static const float kColliderRadius;				// コライダーの半径

	static const int kInitialHP;				// 初期体力
	static const int kInitialAttackPower;		// 初期攻撃力

	static const MyBase::Vector3 kEmitterSize;		// エミッターサイズ
	static const ParticleSystem::ParticleGroupData kHitEffectParams;		// ヒットエフェクトパーティクルパラメータ
	static const ParticleSystem::ParticleGroupData kHitEffectRingParams;	// ヒットエフェクトパーティクルパラメータ(リング)

#ifdef _DEBUG
	static const float kImGuiDragSpeed;				// ImGuiドラッグ速度
	static const MyBase::ScopeF kTranslateScope;	// 平行移動範囲
	static const MyBase::ScopeF kRotateScope;		// 回転範囲
	static const MyBase::ScopeF kScaleScope;		// スケール範囲
#endif // _DEBUG
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
	/// 死亡リアクションの開始
	/// </summary>
	void DeadReactionStart();

	/// <summary>
	/// 死亡リアクションの更新
	/// </summary>
	void DeadReactionUpdate();

#ifdef _DEBUG
	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw() override;
#endif // _DEBUG

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 攻撃可能かどうか
	/// </summary>
	/// <returns>攻撃可能時 true, 不可なら false</returns>
	bool CanAttack();

	/// <summary>
	/// 弾生成
	/// </summary>
	void SpawnBullet();

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

	/// <summary>
	/// ワールド座標を設定
	/// </summary>
	/// <param name="pos">座標の設定</param>
	void SetWorldPosition(const MyBase::Vector3& pos) { object_->SetTranslate(pos); object_->Update(); }

private:	/// メンバ変数
	// 敵の弾リスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	// パーティクルエミッター
	std::unique_ptr<ParticleEmitter> particleEmitter_ = nullptr;

	// ステータス
	int hp_;
	int attackPower_;
	bool isDead_ = false;
	std::unique_ptr<EnemyBaseState> state_;

	// 演出系
	float damageReactionTimer_ = 0.0f;
	float deadReactionTimer_ = 0.0f;

	// 敵の攻撃クールタイム
	float attackCoolTime_ = 0.0f;		// 現在のクールタイム
	
	// 弾の発射位置
	MyBase::Vector3 bulletSpawnPosition_ = { 0.0f, 0.0f, -1.0f }; // 敵の前方に発射

	// プレイヤー
	Player* player_ = nullptr;
};