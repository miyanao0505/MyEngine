#pragma once
#include <string>

/// <summary>
/// 文字コードユーティリティ
/// </summary>
namespace StringUtility
{
	/// <summary>
	/// stringをwstringに変換する
	/// </summary>
	/// <param name="str">変換元の文字列</param>
	/// <returns>変換後の文字列</returns>
	std::wstring ConvertString(const std::string& str);

	/// <summary>
	/// wstring から string へ変換する関数
	/// </summary>
	/// <param name="str">変換元の文字列</param>
	/// <returns>変換後の文字列</returns>
	std::string ConvertString(const std::wstring& str);
}
