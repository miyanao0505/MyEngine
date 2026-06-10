#include "Skydome.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include <imgui.h>
#include <numbers>

using namespace std;
using namespace numbers;

#pragma region 定数
#ifdef _DEBUG
const float Skydome::kImGuiDragSpeed = 0.01f;
const MyBase::ScopeF Skydome::kTranslateScope = { -100.0f, 100.0f };
const MyBase::ScopeF Skydome::kRotateScope = { -pi_v<float>, pi_v<float> };
const MyBase::ScopeF Skydome::kScaleScope = { 0.01f, 10.0f };
#endif // _DEBUG
#pragma endregion

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

	ModelManager::GetInstance()->LoadModel("skydome", "skydome.obj");
	object_ = std::make_unique<Object3d>();

	object_->Initialize("skydome.obj");
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
		ImGui::DragFloat3("Translate", &transform.translate.x, kImGuiDragSpeed, kTranslateScope.min, kTranslateScope.max);
		// 回転
		ImGui::DragFloat3("Rotate", &transform.rotate.x, kImGuiDragSpeed, kRotateScope.min, kRotateScope.max);
		// 拡縮
		ImGui::DragFloat3("Scale", &transform.scale.x, kImGuiDragSpeed, kScaleScope.min, kScaleScope.max);
		object_->SetTransform(transform);

		ImGui::Text("\n");
	}
	ImGui::PopID();
}
#endif // _DEBUG


