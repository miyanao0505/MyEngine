#pragma once
#include "DirectXBase.h"

/// <summary>
/// 3Dモデル描画に共通するDirectXリソースを管理する基盤クラス。
/// </summary>
class ModelBase
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

public:	// getter
	/// <summary>
	/// DirectX の基盤クラスへの参照を取得
	/// </summary>
	/// <returns>DirectXBase インスタンスへのポインタ</returns>
	DirectXBase* GetDxBase() const { return dxBase_; }

private:	// メンバ変数
	// DirectXBase
	DirectXBase* dxBase_ = nullptr;
};