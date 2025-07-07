#include "WorldTransform.h"

// 初期化
void WorldTransform::Initialize()
{
	// 初期化
	position_ = { 0.0f, 0.0f, 0.0f };
	rotation_ = { 0.0f, 0.0f, 0.0f };
	scale_ = { 1.0f, 1.0f, 1.0f };
	worldMatrix_ = Matrix::MakeIdentity4x4();
	parent_ = nullptr;
	isDirty_ = true; // 初期状態ではワールド行列を更新する必要がある
}

// ワールド行列を更新
void WorldTransform::UpdateWorldMatrix()
{
	if (!isDirty_) {
		return; // 既に更新されている場合は何もしない
	}

	// ワールド行列を計算
	worldMatrix_ = Matrix::MakeAffineMatrix(scale_, rotation_, position_);

	// 親が存在する場合は、親のワールド行列を掛け合わせる
	if (parent_) {
		parent_->UpdateWorldMatrix(); // 親のワールド行列を更新
		worldMatrix_ = Matrix::Multiply(parent_->GetWorldMatrix(), worldMatrix_);
	}
	
	isDirty_ = false; // 更新完了フラグをリセット
}
