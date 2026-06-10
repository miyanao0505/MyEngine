#pragma once
#include "Sprite.h"
#include "BaseObject.h"
#include "MyBase.h"
#include <memory>

/// 前方宣言
class Enemy;

/// <summary>
/// レティクルクラス
/// </summary>
class Reticle : public BaseObject {
private:
#pragma region 定数
	static constexpr float kDistance = 100.0f;
	static const MyBase::Vector3 kInitialScale;

#ifdef _DEBUG
	static const float kDebugMoveSpeed;
	static const MyBase::ScopeF kTranslateScope;
	static const MyBase::ScopeF kRotateScope;
	static const MyBase::ScopeF kScaleScope;
#endif // _DEBUG

#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Reticle() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Reticle() = default;

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
	/// デバッグ描画
	/// </summary>
	void DebugDraw() override;
#endif // _DEBUG

	/// <summary>
	/// 
	/// </summary>
	/// <param name="playerPos"></param>
	/// <param name="forward"></param>
	void FollowPlayer(const MyBase::Vector3& playerPos, const MyBase::Vector3& forward);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="target"></param>
	void LockOn(Enemy* target);

	/// <summary>
	/// 
	/// </summary>
	void ReleaseLockOn();

public:	// getter
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	MyBase::Vector3 GetWorldPosition() const override { return BaseObject::GetWorldPosition(); }

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsLockOn() const { return target_ != nullptr; }

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	Enemy* GetTarget() const { return target_; }
private:	// メンバ関数
	

private:	// メンバ変数
	Enemy* target_ = nullptr;

	MyBase::Vector3 position_;

	bool isVisible_ = true;
};