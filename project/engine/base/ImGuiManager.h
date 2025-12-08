#pragma once
#include <WindowsAPI.h>
#include <DirectXBase.h>
#include <SrvManager.h>

/// <summary>
/// ImGui の初期化・描画制御を行う管理クラス。
/// </summary>
class ImGuiManager
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WindowsAPI* winBase, SrvManager* srvManager);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 画面への描画
	/// </summary>
	void Draw();

private:	// メンバ変数
	// DxBase
	DirectXBase* dxBase_ = nullptr;
	// ImGui用のデスクリプタヒープを管理するSRVマネージャ
	SrvManager* srvManager_ = nullptr;
};