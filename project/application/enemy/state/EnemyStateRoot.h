#pragma once
#include "EnemyBaseState.h"

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

