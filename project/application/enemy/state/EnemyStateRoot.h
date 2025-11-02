#pragma once
#include "EnemyBaseState.h"

/// <summary>
/// EnemyStateRootクラス
/// 敵キャラクター(Enemy)の初期状態(ルートステート)を表すクラス。
/// </summary>
class EnemyStateRoot : public EnemyBaseState
{
public:
	// コンストラクタ
	EnemyStateRoot(Enemy* enemy) : EnemyBaseState("Root", enemy) {};
	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
};

