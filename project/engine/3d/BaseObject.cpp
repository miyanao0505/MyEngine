#include "BaseObject.h"

using namespace std;

// 初期化
void BaseObject::Initialize(const std::string& modelPath)
{
	object_ = make_unique<Object3d>();
	object_->Initislize(modelPath);
}

// 更新
void BaseObject::Update()
{
	object_->Update();
}

// 描画
void BaseObject::Draw()
{
	object_->Draw();
}

// 衝突判定処理
void BaseObject::OnCollision(Collider* other)
{
	if (collider_) {
		collider_->OnCollision(other);
	}
}
