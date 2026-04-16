#pragma once

/// <summary>
/// デバッグラインカテゴリ
/// </summary>
enum class DebugLineCategory {
	kCollision,	// コリジョンの当たり判定
	kRail,		// レールカメラの軌道
	kOther,		// その他のデバッグライン

	kCount,		// 使用禁止
};