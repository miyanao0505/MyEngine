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
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("object"))
	{
		if (ImGui::TreeNode("Transform"))
		{
			MyBase::Transform transform = { object_->GetScale(), object_->GetRotate(), object_->GetTranslate() };

			// 移動
			ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f, -100.0f, 100.0f);
			// 回転
			ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f, -3.14f, 3.14f);
			// 拡縮
			ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f, 0.01f, 10.0f);
			object_->SetTransform(transform);
			
			ImGui::TreePop();
		}
		//ImGui::Text("\n");
		if (ImGui::TreeNode("Material"))
		{
			MyBase::ModelMaterial* materialData = object_->GetModel()->GetModelMaterial();

			// 色
			ImGui::ColorEdit4("color", &materialData->color.x);
			// 光沢度
			ImGui::SliderFloat("shininess", &materialData->shininess, 0.0f, 1.0f);
			// 反射強度
			ImGui::SliderFloat("reflectivity", &materialData->reflectivity, 0.0f, 1.0f);
			// 有効かどうか
			bool isEnable = materialData->enableLighting;
			ImGui::Checkbox("enableLighting", &isEnable);
			materialData->enableLighting = isEnable;
			object_->GetModel()->SetModelMaterial(materialData);

			ImGui::TreePop();
		}
	}
	ImGui::PopID();
}
#endif // _DEBUG


// 衝突判定処理
void BaseObject::OnCollision(Collider* other)
{
	if (collider_) {
		collider_->OnCollision(other);
	}
}
