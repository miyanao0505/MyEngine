#pragma once
#include "BaseObject.h"
#include <list>
#include "TextureManager.h"
#include "BaseBullet.h"
#include "Reticle.h"

/// <summary>
/// プレイヤーキャラクターを表すクラス。
/// 自機の移動・回転・攻撃・衝突処理など、ゲーム中の主要な操作と挙動を管理します。
/// </summary>
class Player : public BaseObject
{
private:
#pragma region 定数
	static constexpr float kDamageReactionDuration = 0.1f;	// ダメージリアクション時間
	static constexpr float kDeadReactionDuration = 1.0f;	// 死亡リアクション時間
	static constexpr float kAttackCoolTime = 0.5f;							// 攻撃クールタイム

	static constexpr float kMoveSpeed = 10.0f;		// プレイヤーの移動速度
	static const int kInitialHP;					// 初期HP
	static const int kInitialAttackPower;			// 初期攻撃力
	
	static constexpr float kBulletSpeed = 150.0f;							// 弾の移動速度
	static constexpr int kMaxBulletCount = 10;								// 最大弾数
	static constexpr float kBulletDrawDistance = 500.0f;					// 弾の描画距離
	static constexpr MyBase::Vector3 kBulletOffset = {0.0f, 0.0f, 1.0f};	// 弾の発射位置オフセット

	static const MyBase::Vector3 kInitialScale;		// 初期スケール
	static const float kColliderRadius;				// コライダー半径

#ifdef _DEBUG
	static const float kDebugMoveSpeed;	// デバッグ用移動速度
	static const MyBase::ScopeF kTranslateScope;	// デバッグ用平行移動範囲
	static const MyBase::ScopeF kRotateScope;		// デバッグ用回転範囲
	static const MyBase::ScopeF kScaleScope;		// デバッグ用スケール範囲
#endif // _DEBUG
#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">初期座標</param>
	void Initialize(const MyBase::Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

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
	/// 移動処理
	/// </summary>
	void ReadMoveInput();

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
	void OnCollision(Collider* other) override;

public:	// getter
	/// <summary>
	/// 攻撃力を取得
	/// </summary>
	/// <returns>攻撃力を取得</returns>
	int GetAttackPower() const { return attackPower_; }

	/// <summary>
	/// 死亡フラグを取得
	/// </summary>
	/// <returns>死亡フラグを取得</returns>
	bool IsDead() const { return isDead_; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	MyBase::Vector3 GetWorldPosition() const override { return BaseObject::GetWorldPosition(); };

	/// <summary>
	/// 移動入力を取得
	/// </summary>
	/// <returns>移動入力</returns>
	MyBase::Vector2 GetMoveInput() const { return moveInput_; }

	/// <summary>
	/// 移動速度を取得
	/// </summary>
	/// <returns>移動速度</returns>
	float GetMoveSpeed() const { return kMoveSpeed; }

public:	// setter
	/// <summary>
	/// 攻撃力を設定
	/// </summary>
	/// <param name="attackPower">攻撃力</param>
	void SetAttackPower(int attackPower) { attackPower_ = attackPower; }

	/// <summary>
	/// ワールド座標を設定
	/// </summary>
	/// <param name="pos">新たな座標</param>
	void SetWorldPosition(const MyBase::Vector3& pos);

	/// <summary>
	/// レティクルを設定
	/// </summary>
	/// <param name="reticle">レティクルのポインタ</param>
	void SetReticle(Reticle* reticle) { reticle_ = reticle; }

private:	// メンバ変数		
	// ステータス
	int hp_;
	int attackPower_ = 1;
	bool isDead_ = false;

	// 演出系
	float damageReactionTimer_;
	float deadReactionTimer_;
	
	MyBase::Vector2 moveInput_;	// 入力意図(-1～1)
	MyBase::Vector3 externalPosition_; // 外部からの位置影響（ノックバックなど）

	// プレイヤーの攻撃クールタイム
	float attackCoolTime_ = 0;		// 現在のクールタイム

	// 弾の発射位置
	MyBase::Vector3 bulletSpawnPosition_ = { 0.0f, 0.0f, 1.0f }; // プレイヤーの前方に発射

	// 照準用のレティクル
	Reticle* reticle_ = nullptr;
};