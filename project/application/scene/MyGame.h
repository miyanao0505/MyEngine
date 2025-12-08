#pragma once
#include "MNFramework.h"

/// <summary>
/// ゲーム全体のエントリーポイントとなるクラスで、
/// MNFrameworkを継承し、アプリケーション全体の初期化・更新・描画・終了処理を統括します。
/// </summary>
class MyGame : public MNFramework
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:	// メンバ変数

};