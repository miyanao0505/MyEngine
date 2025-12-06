#include "RailCamera.h"
#include <imgui.h>
#include "MyTools.h"

/// 初期化
void RailCamera::Initialize(const std::vector<MyBase::Vector3>& points)
{
	// 制御点リストの設定
	controlPoints_ = points;
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
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("RailCamera"))
	{
		// レールポイント&ラインの表示
		ImGui::Checkbox("Debug Mode", &isDebugMode_);

		ImGui::Text("\n");
	}
	ImGui::PopID();
}
#endif // _DEBUG

/// レールに沿って移動
void RailCamera::MoveAlongRail(float deltaTime)
{
	if (nextPointIndex_ >= controlPoints_.size()) return;

	lerpT_ += deltaTime * railSpeed_;

	if (lerpT_ >= 1.0f) {
		// 次の区間へ
		lerpT_ = 0.0f;
		currentPointIndex_++;
		nextPointIndex_++;
		if (nextPointIndex_ >= controlPoints_.size()) return;
	}

	railPosition_ = MyTools::Lerp(controlPoints_[currentPointIndex_], controlPoints_[nextPointIndex_], lerpT_);
}