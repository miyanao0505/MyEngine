#pragma once
#include "EnemyBaseState.h"

/// <summary>
/// EnemyStateRootクラス
/// 敵キャラクター(Enemy)の初期状態(ルートステート)を表すクラス。
/// </summary>
class EnemyStateRoot : public EnemyBaseState
{
public:	// メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy">敵キャラクターのポインタ</param>
	EnemyStateRoot(Enemy* enemy) : EnemyBaseState("Root", enemy) {};
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
};

