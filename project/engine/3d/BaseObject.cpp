#include "BaseObject.h"
#include <imgui.h>

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

#ifdef _DEBUG
// デバック描画
void BaseObject::DebugDraw()
{
	if (ImGui::CollapsingHeader("object"))
	{
		MyBase::Transform transform = { object_->GetScale(), object_->GetRotate(), object_->GetTranslate() };

		// 移動
		ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f, -100.0f, 100.0f);
		// 回転
		ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f, -3.14f, 3.14f);
		// 拡縮
		ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f, 0.01f, 10.0f);
		object_->SetTransform(transform);

		ImGui::Text("\n");
	}
}
#endif // _DEBUG


// 衝突判定処理
void BaseObject::OnCollision(Collider* other)
{
	if (collider_) {
		collider_->OnCollision(other);
	}
}
