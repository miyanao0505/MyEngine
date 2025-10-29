#pragma once
#include <string>

/// <summary>
/// 文字コードユーティリティ
/// </summary>
namespace StringUtility
{
	// stringをwstringに変換する
	std::wstring ConvertString(const std::string& str);

	// wstring から string へ変換する関数
	std::string ConvertString(const std::wstring& str);
}
