#pragma once
#include <string>

/// <summary>
/// 出力ウィンドウに文字を出す
/// </summary>
namespace Logger
{
	/// <summary>
	/// ログ出力
	/// </summary>
	/// <param name="message">メッセージ</param>
	void Log(const std::string& message);
}
