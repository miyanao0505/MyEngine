#include "FollowCamera.h"
#include "CameraManager.h"
#include "MyTools.h"
#include "imgui.h"

void FollowCamera::Initialize() {
	CameraManager::GetInstance()->SetCamera("FollowCamera");
	CameraManager::GetInstance()->FindCamera("FollowCamera");
	camera_ = CameraManager::GetInstance()->GetCamera();

	// 注視点からのオフセットを設定
	offset_ = { 0.0f, 7.50f, -40.0f };
	// カメラの方向を設定
	directional_ = { 0.0f, 1.0f, 0.0f };

	Update();
}

void FollowCamera::Update() {
	if (camera_ == nullptr || player_ == nullptr) {
		return;
	}
	// 注視点の座標を取得
	MyBase::Vector3 targetPos = player_->GetWorldPosition();
	// カメラの現在の座標を取得
	MyBase::Vector3 cameraPos = camera_->GetTranslate();
	//カメラの位置をプレイヤーの後ろに設定
	cameraPos = MyTools::Add(targetPos, offset_);
	camera_->SetTranslate(cameraPos);
	// カメラの向きをプレイヤーの方向に設定
	MyBase::Vector3 direction = MyTools::Subtract(targetPos, cameraPos);
	direction = MyTools::Normalize(direction);
	// カメラの回転を計算(見下ろし型)
	float pitch = -atan2(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z));
	camera_->SetRotate({ pitch, 0.0f, 0.0f });
	// カメラの更新
	camera_->Update();
}

void FollowCamera::Draw() {

}

#ifdef _DEBUG
// デバック用の描画
void FollowCamera::DebugDraw() {
	// カメラ
	if (ImGui::CollapsingHeader("FollowCamera"))
	{
		// オフセット
		MyBase::Vector3 offset = offset_;

		// ImGuiを用いた変更
		ImGui::DragFloat3("offset", &offset.x, 0.05f);

		// 変更を反映
		offset_ = offset;

		ImGui::Text("\n");
	}
}
#endif // _DEBUG