#pragma once
#include "MNFramework.h"

/// <summary>
/// ゲーム全体のエントリーポイントとなるクラスで、
/// MNFrameworkを継承し、アプリケーション全体の初期化・更新・描画・終了処理を統括します。
/// </summary>
class MyGame : public MNFramework
{
private:
#pragma region 定数
	static constexpr MyBase::Vector3 kDefaultCameraTranslate{ 0.0f, 7.0f, -15.0f };
	static constexpr MyBase::Vector3 kDefaultCameraRotate{ 0.4f, 0.0f, 0.0f };
	static constexpr MyBase::Vector3 kSubCameraTranslate{ -15.0f, 7.0f, 0.0f };
	static constexpr MyBase::Vector3 kSubCameraRotate{ 0.3f, 1.57f, 0.0f };
#pragma endregion

public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const wchar_t* windowTitle) override;

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