#include "FollowCamera.h"
#include "CameraManager.h"
#include "MyTools.h"
#include "imgui.h"

/// 初期化
void FollowCamera::Initialize()
{
	CameraManager::GetInstance()->AddCamera("FollowCamera");
	CameraManager::GetInstance()->SetCamera("FollowCamera");
	camera_ = CameraManager::GetInstance()->GetCamera();

	// 注視点からのオフセットを設定
	offset_ = kDefaultOffset;
	// カメラの方向を設定
	upDirection_ = kWorldUp;
}

/// 更新
void FollowCamera::Update([[maybe_unused]] float deltaTime)
{
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

/// 描画
void FollowCamera::Draw()
{

}

/// 追従処理を更新する
void FollowCamera::UpdateFollow(float deltaTime)
{
	if (!player_) return;

	target_ = player_->GetWorldPosition();
	MyBase::Vector3 cameraPos = camera_->GetTranslate();
	
	// プレイヤーからの相対位置
	MyBase::Vector3 basePos = MyTools::Add(target_, offset_);

	// スムージング
	cameraPos = MyTools::Lerp(cameraPos, basePos, deltaTime * followSmooth_);

	// 理想位置の保存
	currentFollowPosition_ = cameraPos;
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
		ImGui::DragFloat3("offset", &offset.x, kOffsetDragSpeed);

		// 変更を反映
		offset_ = offset;

		ImGui::Text("\n");
	}
	ImGui::PopID();
}
#endif // _DEBUG