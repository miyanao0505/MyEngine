#include "FollowCamera.h"
#include "CameraManager.h"
#include "MyTools.h"
#include "imgui.h"

/// 初期化
void FollowCamera::Initialize(Camera* camera)
{
	camera_ = camera;
	offset_ = kDefaultOffset;
	followSmooth_ = kDefaultFollowSmooth;

	// 注視点からのオフセットを設定
	offset_ = kDefaultOffset;
	// カメラの方向を設定
	upDirection_ = kWorldUp;
}

/// 更新
void FollowCamera::Update([[maybe_unused]] float deltaTime)
{
	if (camera_ == nullptr) return;

	// 追従処理の更新
	UpdateFollowTranslate(deltaTime);

	// 注視点の更新
	UpdateLookAtTarget();

	// カメラの更新
	camera_->Update();
}

/// 追従処理を更新する
void FollowCamera::UpdateFollowTranslate(float deltaTime)
{
	if (!isTargetSet_) return;

	// 理想のカメラ位置
	MyBase::Vector3 idealPos = MyTools::Add(target_, offset_);

	// 現在位置
	MyBase::Vector3 currentPos = camera_->GetTranslate();

	// スムージング
	MyBase::Vector3 newPos = MyTools::Lerp(currentPos, idealPos, deltaTime * followSmooth_);

	// カメラ位置反映
	camera_->SetTranslate(newPos);

	// 理想位置の保存
	currentFollowPosition_ = newPos;
}

/// 注視点の更新
void FollowCamera::UpdateLookAtTarget()
{
	// カメラの向きをプレイヤーの方向に設定
	MyBase::Vector3 direction = MyTools::Subtract(target_, currentFollowPosition_);
	direction = MyTools::Normalize(direction);
	// カメラの回転を計算(見下ろし型)
	float horizontalLength = sqrt(direction.x * direction.x + direction.z * direction.z);
	float pitch = -atan2(direction.y, horizontalLength);
	camera_->SetRotate({ pitch, 0.0f, 0.0f });
}

#ifdef _DEBUG
/// デバック用の描画
void FollowCamera::DebugDraw() {
	// フォローカメラ
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("FollowCamera"))
	{
		// オフセット
		MyBase::Vector3 offset = offset_;
		
		// ImGuiを用いた変更
		ImGui::DragFloat3(kOffsetLabel, &offset.x, kOffsetDragSpeed);

		// 変更を反映
		offset_ = offset;

		ImGui::Text("\n");
	}
	ImGui::PopID();
}
#endif // _DEBUG