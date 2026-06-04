#include "FollowCamera.h"
#include "CameraManager.h"
#include "MyTools.h"
#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

using namespace MyBase;

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

	Vector3 offsetWorld = MyTools::Add(MyTools::Add(MyTools::Multiply(offset_.x, basisRight_), MyTools::Multiply(offset_.y, basisUp_)), MyTools::Multiply(offset_.z, basisForward_));

	// 理想のカメラ位置
	Vector3 idealPos = MyTools::Add(target_, offsetWorld);

	// 現在位置
	Vector3 currentPos = camera_->GetTranslate();

	// スムージング
	Vector3 newPos = MyTools::Lerp(currentPos, idealPos, deltaTime * followSmooth_);

	// カメラ位置反映
	camera_->SetTranslate(newPos);

	// 理想位置の保存
	currentFollowPosition_ = newPos;
}

/// 注視点の更新
void FollowCamera::UpdateLookAtTarget()
{
	// カメラの向きをプレイヤーの方向に設定
	Vector3 direction = MyTools::Subtract(target_, currentFollowPosition_);
	direction = MyTools::Normalize(direction);
	float horizontalLength = sqrt(direction.x * direction.x + direction.z * direction.z);

	// カメラの回転を計算(見下ろし型)
	float pitch = -atan2(direction.y, horizontalLength);
	float yaw = atan2(direction.x, direction.z);
	camera_->SetRotate({ pitch, yaw, 0.0f });
}

#ifdef _DEBUG
/// デバック用の描画
void FollowCamera::DebugDraw() {
	// フォローカメラ
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("FollowCamera"))
	{
		// オフセット
		Vector3 offset = offset_;
		
		// ImGuiを用いた変更
		ImGui::DragFloat3(kOffsetLabel, &offset.x, kOffsetDragSpeed);

		// 変更を反映
		offset_ = offset;

		ImGui::Text("\n");
	}
	ImGui::PopID();
}
#endif // _DEBUG