#include "RailCamera.h"

// コンストラクタ
RailCamera::RailCamera() 
{
	Initialize();
}

RailCamera::~RailCamera()
{
}

// 初期化
void RailCamera::Initialize()
{
	ofSet_ = { 0.0f, 0.0f, 0.0f };
	directional_ = { 0.0f, 0.0f, 0.0f };
}

// 更新
void RailCamera::Update()
{
}
