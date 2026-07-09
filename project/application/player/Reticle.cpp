#include "Reticle.h"
#include "Input.h"
#include "CameraManager.h"
#include "MyTools.h"
#include "Matrix.h"
#include "WindowsAPI.h"

using namespace MyBase;

/// 初期化
void Reticle::Initialize() {
	// スクリーン座標の初期化
	screenPosition_ = Input::GetInstance()->GetMousePosition();

	// スプライトの初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("2DReticle.png");
	sprite_->SetAnchorPoint({ 0.5f, 0.5f });
	sprite_->SetPosition(screenPosition_);
	sprite_->SetSize({ 50.0f, 50.0f });
}

/// 更新
void Reticle::Update() {
	screenPosition_ = Input::GetInstance()->GetMousePosition();

	UpdateWorldPosition();

	sprite_->SetPosition(screenPosition_);

	if (sprite_) {
		sprite_->Update();
	}
}

/// 描画
void Reticle::Draw() {
	if (sprite_) {
		sprite_->Draw();
	}
}

/// ワールド座標をスクリーン座標に変換
void Reticle::UpdateWorldPosition() {
	auto camera = CameraManager::GetInstance()->GetCamera();

	Matrix4x4 matViewport = Matrix::MakeViewportMatrix(0.0f, 0.0f, WindowsAPI::kClientWidth, WindowsAPI::kClientHeight, 0.0f, 1.0f);
	Matrix4x4 matVPV = Matrix::Multiply(camera->GetViewProjectionMatrix(), matViewport);
	Matrix4x4 matInverseVPV = Matrix::Inverse(matVPV);

	Vector3 posNear = Vector3{ screenPosition_.x, screenPosition_.y, 0.0f };
	Vector3 posFar = Vector3{ screenPosition_.x, screenPosition_.y, 1.0f };

	posNear = Matrix::Transform(posNear, matInverseVPV);
	posFar = Matrix::Transform(posFar, matInverseVPV);

	Vector3 mouseDirection = MyTools::Normalize(MyTools::Subtract(posFar, posNear));
	worldPosition_ = MyTools::Add(posNear, MyTools::Multiply(kDistance3DReticle, mouseDirection));
}