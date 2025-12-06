#pragma once

/// <summary>
/// DirectXリソースのリーク(未解放)検出用クラス
/// DirectX(D3D12)関連のリソースリークを検出するためのデバッグ補助クラスです。
/// </summary>
class D3DResourceLeakChecker
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~D3DResourceLeakChecker();
};