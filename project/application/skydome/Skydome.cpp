#include "Skydome.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include <imgui.h>

Skydome::Skydome()
{
	
}

Skydome::~Skydome()
{
}

/// 初期化
void Skydome::Initialize(const std::string& filePath, MyBase::Vector3 position, MyBase::Vector3 scale)
{
	TextureManager::GetInstance()->LoadTexture(filePath);

	ModelManager::GetInstance()->LoadModel("debug/sphere", "sphere.obj");
	object_ = std::make_unique<Object3d>();

	object_->Initislize("sphere.obj");
	object_->SetTexture(filePath);
	object_->SetScale(scale);
	object_->SetTranslate(position);
}

void Skydome::Update()
{
	object_->Update();
}

void Skydome::Draw()
{
	object_->Draw();
}

#ifdef _DEBUG
void Skydome::DebugDraw()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Skydome"))
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
	ImGui::PopID();
}
#endif // _DEBUG


