#pragma once
#include <cstdint>

/// <summary>
/// マウスボタン定義
/// 入力デバイス共有で使用する列挙型
/// </summary>
enum class MouseButton : uint8_t
{
	Left = 0,
	Right = 1,
	Middle = 2,
	Side1 = 3,
	Side2 = 4,
};