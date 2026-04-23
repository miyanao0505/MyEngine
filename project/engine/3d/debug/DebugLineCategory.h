#pragma once

#ifdef _DEBUG
/// <summary>
/// デバッグラインカテゴリ
/// </summary>
enum class DebugLineCategory {
	collision,	// コリジョンの当たり判定
	rail,		// レールカメラの軌道
	other,		// その他のデバッグライン

	kCount,		// 使用禁止
};

namespace Miyazawa {
	/// <summary>
	/// デバッグラインカテゴリを文字列に変換
	/// </summary>
	/// <param name="category">デバッグラインカテゴリ</param>
	/// <returns>カテゴリの文字列表現</returns>
	static const char* ToString(DebugLineCategory category) {
		switch (category) {
		case DebugLineCategory::collision:	return "Collision";
		case DebugLineCategory::rail:		return "Rail";
		case DebugLineCategory::other:		return "Other";
		default:							return "Unknown";
		}
	}
}
#endif // _DEBUG