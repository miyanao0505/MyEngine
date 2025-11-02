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
public:
	// コンストラクタ
	EnemyBaseState(const std::string& name, Enemy* enemy) : name_(name), enemy_(enemy) {};

	// デストラクタ
	virtual ~EnemyBaseState() {};

	// ステートの初期化
	virtual void Initialize() = 0;
	// ステートの更新
	virtual void Update() = 0;

#ifdef _DEBUG
	// デバックログ出力
	virtual void DebugLog();
#endif // _DEBUG

protected:
	std::string name_;
	Enemy* enemy_ = nullptr;

};

