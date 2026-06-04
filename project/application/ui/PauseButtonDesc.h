#pragma once
#include "PauseButtonType.h"
#include "MyBase.h"

struct PauseButtonDesc {
	PauseButtonType type;
	std::string texturePath;
	MyBase::Vector2 position;
};