#include "RailCamera.h"
#include <imgui.h>
#include "MyTools.h"
#include "BaseObject.h"
#include "ModelManager.h"

using namespace std;
using namespace MyBase;

/// 初期化
void RailCamera::Initialize() {
	lerpT_ = kLerpStart;
	speed_ = kDefaultRailSpeed;
}

/// 更新
void RailCamera::Update(float deltaTime) {
	// レールに沿って移動
	MoveAlongRail(deltaTime);
}

#ifdef _DEBUG
/// デバッグ更新
void RailCamera::DebugUpdate() {
	// レールポイント&ラインの表示
	ImGui::Checkbox("Debug Mode", &isDebugMode_);
	ImGui::Text("\n");
}

/// デバッグ描画
void RailCamera::DebugDraw() {
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("RailCamera")) 	{
		DebugUpdate();

		if (isDebugMode_) {
			ImGui::Text("RailCamera Debug Mode ON");

			// 制御点リストの表示
			for (size_t i = 0; i < controlPoints_.size(); i++) {
				Vector3& point = controlPoints_[i];
				ImGui::Text("Point %zu: (%.2f, %.2f, %.2f)", i, point.x, point.y, point.z);
			}
		} else {
			ImGui::Text("RailCamera Debug Mode OFF");
		}

		ImGui::Text("\n");
	}
	ImGui::PopID();
}
#endif // _DEBUG

/// レールに沿って移動
void RailCamera::MoveAlongRail(float deltaTime) {
	if (controlPoints_.size() < 4) return;

	// 進行度更新
	currentDistance_ += speed_ * deltaTime;
	currentDistance_ = MyTools::Clamp(currentDistance_, 0.0f, totalLength_);

	// ループorクランプ
	lerpT_ = DistanceToT(currentDistance_);

	// 現在位置
	position_ = MyTools::CatmullRomPosition(controlPoints_, lerpT_);

	float offset = 0.001f;

	float tPrev = MyTools::Clamp(lerpT_ - offset, 0.0f, 1.0f);
	float tNext = MyTools::Clamp(lerpT_ + offset, 0.0f, 1.0f);

	Vector3 prev = MyTools::CatmullRomPosition(controlPoints_, tPrev);
	Vector3 next = MyTools::CatmullRomPosition(controlPoints_, tNext);

	// 進行方向
	Vector3 diff = MyTools::Subtract(next, prev);
	if (MyTools::Length(diff) > 0.0001f) {
		forward_ = MyTools::Normalize(diff);
	}
}

/// アーク長テーブルの構築
void RailCamera::BuildArcTable() {
	arcTable_.clear();
	float length = 0.0f;

	Vector3 prev = MyTools::CatmullRomPosition(controlPoints_, 0.0f);

	arcTable_.push_back({ 0.0f, 0.0f });

	for (uint16_t i = 1; i <= kDiv; i++) {
		float t = (float)i / kDiv;
		Vector3 cur = MyTools::CatmullRomPosition(controlPoints_, t);

		length += MyTools::Length(MyTools::Subtract(cur, prev));
		arcTable_.push_back({ t, length });

		prev = cur;
	}

	totalLength_ = length;
}

/// 距離から区間内の補間値を求める
float RailCamera::DistanceToT(float distance) {
	size_t left = 0;
	size_t right = arcTable_.size() - 1;

	while (left < right) {
		size_t mid = (left + right) / 2;

		if (arcTable_[mid].length < distance) {
			left = mid + 1;
		} else {
			right = mid;
		}
	}

	size_t i = left;

	if (i == 0) return arcTable_[0].t;

	float l0 = arcTable_[i - 1].length;
	float l1 = arcTable_[i].length;

	float t0 = arcTable_[i - 1].t;
	float t1 = arcTable_[i].t;

	float ratio = (distance - l0) / (l1 - l0);
	return t0 + (t1 - t0) * ratio;
}

/// レールデータの取得
MyBase::RailData RailCamera::GetRailData() const {
	MyBase::RailData railData{};
	railData.position = position_;
	railData.forward = forward_;
	return railData;
}

/// レールの制御点リストを設定
void RailCamera::SetRailPoints(const std::vector<MyBase::Vector3>& points) {
	controlPoints_ = points;

	if (controlPoints_.size() >= kMinCatmullRomPoints) {
		BuildArcTable();
	}
}