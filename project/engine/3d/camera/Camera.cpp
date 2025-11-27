#include "Camera.h"
#include "Matrix.h"
#include "WindowsAPI.h"

// コンストラクタ
Camera::Camera()
	: transform_({ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} })
	, fovY_(0.45f)
	// 画面サイズからアスペクト比を求める
	, aspectRatio_(float(WindowsAPI::kClientWidth) / float(WindowsAPI::kClientHeight))
	, nearClip_(0.1f)
	, farClip_(100.0f)
	// 初期状態からワールド行列・ビュー行列・射影行列を事前計算
	, worldMatrix_(Matrix::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate))
	// カメラのビュー行列はワールド行列の逆行列(カメラ座標系を構築するため)
	, viewMatrix_(Matrix::Inverse(worldMatrix_))
	, projectionMatrix_(Matrix::MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_))
	// 描画で頻用する合成行列を先に作成
	, viewProjectionMatrix_(Matrix::Multiply(viewMatrix_, projectionMatrix_))
{}

// 更新
void Camera::Update()
{
	// アフィン返還行列を計算
	worldMatrix_ = Matrix::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	// worldMatrixの逆行列
	viewMatrix_ = Matrix::Inverse(worldMatrix_);
	// プロジェクション行列の更新
	projectionMatrix_ = Matrix::MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);
	// ビュー × プロジェクションの合成行列更新
	viewProjectionMatrix_ = Matrix::Multiply(viewMatrix_, projectionMatrix_);

}
