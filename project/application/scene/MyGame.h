#pragma once
#include "MNFramework.h"

/// <summary>
/// ゲーム全体のエントリーポイントとなるクラスで、
/// MNFrameworkを継承し、アプリケーション全体の初期化・更新・描画・終了処理を統括します。
/// </summary>
class MyGame : public MNFramework
{
public:	// メンバ関数

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;

private:	// メンバ変数

};

