#include "RailFollowSystem.h"

/// 初期化
void RailFollowSystem::Initialize(RailCamera* rail, FollowCamera* follow, Camera* outCamera)
{
	// メンバ変数の設定
	railCamera_ = rail;
	followCamera_ = follow;
	camera_ = outCamera;
}

/// 更新
void RailFollowSystem::Update()
{

}
