#pragma once
#include <string>
#include <memory>

/// 前方宣言
class Enemy;

/// <summary>
/// EnemyBaseStateクラス
/// 敵キャラクター（Enemy）の**行動状態を定義するための基底クラス**。
/// </summary>
class EnemyBaseState
{
public:	// メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">ステート名</param>
	/// <param name="enemy">敵キャラクターのポインタ</param>
	EnemyBaseState(const std::string& name, Enemy* enemy) : name_(name), enemy_(enemy) {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyBaseState() {};

	/// <summary>
	/// ステートの初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// ステートの更新
	/// </summary>
	virtual void Update() = 0;

#ifdef _DEBUG
	/// <summary>
	/// デバックログ出力
	/// </summary>
	virtual void DebugLog();
#endif // _DEBUG

protected:
	std::string name_;
	Enemy* enemy_ = nullptr;

};

