#include "RailCamera.h"
#include <imgui.h>
#include "MyTools.h"
#include "BaseObject.h"
#include "ModelManager.h"

using namespace std;

/// 初期化
void RailCamera::Initialize()
{
	lerpT_ = kLerpStart;
	speed_ = kDefaultRailSpeed;
}

/// 更新
void RailCamera::Update(float deltaTime)
{
	// レールに沿って移動
	MoveAlongRail(deltaTime);
}

#ifdef _DEBUG
/// デバッグ更新
void RailCamera::DebugUpdate()
{
	// レールポイント&ラインの表示
	ImGui::Checkbox("Debug Mode", &isDebugMode_);
	ImGui::Text("\n");
}

/// デバッグ描画
void RailCamera::DebugDraw()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("RailCamera"))
	{
		DebugUpdate();

		if (isDebugMode_) {
			// 制御点リストの表示
			for (size_t i = 0; i < controlPoints_.size(); i++) {
				MyBase::Vector3& point = controlPoints_[i];
				ImGui::Text("Point %zu: (%.2f, %.2f, %.2f)", i, point.x, point.y, point.z);
			}
		}

		ImGui::Text("\n");
	}
	ImGui::PopID();
}
#endif // _DEBUG

/// レールに沿って移動
void RailCamera::MoveAlongRail(float deltaTime)
{
	if (controlPoints_.size() < 4) return;

	// 進行度更新
	lerpT_ += speed_ * deltaTime;

	// ループorクランプ
	lerpT_ = MyTools::Clamp(lerpT_, kLerpStart, kLerpEnd);

	// 現在位置
	position_ = MyTools::CatmullRomPosition(controlPoints_, lerpT_);

	// 少し先の位置
	const float epsilon = 0.001f;
	float tNext = MyTools::Clamp(lerpT_ + epsilon, kLerpStart, kLerpEnd);
	MyBase::Vector3 nextPos = MyTools::CatmullRomPosition(controlPoints_, tNext);

	// 進行方向
	forward_ = MyTools::Normalize(MyTools::Subtract(nextPos, position_));
}

/// レールデータの取得
MyBase::RailData RailCamera::GetRailData() const
{
	MyBase::RailData railData{};
	railData.position = position_;
	railData.forward = forward_;
	return railData;
}