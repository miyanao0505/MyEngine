#pragma once
#include "DirectXBase.h"

/// <summary>
/// 3Dモデル描画に共通するDirectXリソースを管理する基盤クラス。
/// </summary>
class ModelBase
{
public:	// メンバ関数
	// 初期化
	void Initislize();

public:	// getter
	DirectXBase* GetDxBase() const { return dxBase_; }

private:	// メンバ変数
	// DirectXBase
	DirectXBase* dxBase_ = nullptr;

};

