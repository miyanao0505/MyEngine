#pragma once
#include "PauseButtonType.h"
#include "MyBase.h"

/// <summary>
/// ポーズ画面に表示するボタン情報を保持する構造体
/// </summary>
struct PauseButtonDesc {
	PauseButtonType type;
	std::string texturePath;
	MyBase::Vector2 position;
};