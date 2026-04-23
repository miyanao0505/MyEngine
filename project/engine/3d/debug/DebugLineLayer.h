#pragma once
#include <cstdint>
#include "MyBase.h"

#ifdef _DEBUG
/// <summary>
/// デバッグラインレイヤークラス
/// デバッグラインの描画レイヤーを管理するクラス。
/// </summary>
class DebugLineLayer {
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="maxLineCount">ラインの最大数</param>
	void Initialize(uint32_t maxLineCount);

	/// <summary>
	/// フレーム開始時に呼ぶ
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// ラインの追加
	/// </summary>
	/// <param name="line">追加するライン情報</param>
	void AddLine(const MyBase::DebugLine& line);

public:	// getter
	/// <summary>
	/// ライン情報の取得
	/// </summary>
	/// <returns>ライン情報の配列</returns>
	const std::vector<MyBase::DebugLine>& GetLines() const { return lines_; }

private:	// メンバ変数
	std::vector<MyBase::DebugLine> lines_;
	uint32_t maxLineCount_ = 0;
};
#endif // _DEBUG