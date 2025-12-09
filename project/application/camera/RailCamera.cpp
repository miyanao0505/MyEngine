#include "RailCamera.h"
#include <imgui.h>
#include "MyTools.h"
#include "BaseObject.h"
#include "ModelManager.h"

using namespace std;

/// 初期化
void RailCamera::Initialize(const vector<MyBase::Vector3>& points)
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