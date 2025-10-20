#include "LightManager.h"
#include "ModelManager.h"
#include "CameraManager.h"
#include "imgui.h"

LightManager* LightManager::instance = nullptr;

// シングルトンインスタンスの取得
LightManager* LightManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new LightManager;
	}
	return instance;
}

// 終了
void LightManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

// 初期化
void LightManager::Initialize()
{
	dxBase_ = DirectXBase::GetInstance();

	// 平行光源データの作成
	CreateDirectionalLightData();

	// 点光源データの作成
	CreatePointLightData();

	// スポットライトデータの作成
	CreateSpotLightData();
}

// 描画処理
void LightManager::Draw(Object3dBase* object3dBase)
{
	// 平行光源用のCBufferの場所を設定
	object3dBase->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_.Get()->GetGPUVirtualAddress());
	// 点光源用のCBufferの場所を設定
	object3dBase->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(5, pointLightResource_.Get()->GetGPUVirtualAddress());
	// スポットライト用のCBufferの場所を設定
	object3dBase->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(6, spotLightResource_.Get()->GetGPUVirtualAddress());
}

#ifdef _DEBUG
// デバッグ描画
void LightManager::DebugDraw()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Light")) {
		ImGui::PushID("Point");
		if (ImGui::TreeNode("Point Light")) {
			ImGui::ColorEdit4("Color", &pointLightData_->color.x);
			ImGui::DragFloat3("Position", &pointLightData_->position.x, 0.1f);
			ImGui::DragFloat("Intensity", &pointLightData_->intensity, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("Radius", &pointLightData_->radius, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("Decay", &pointLightData_->decay, 0.01f, 0.0f, 10.0f);
			ImGui::TreePop();
		}
		ImGui::PopID();

		ImGui::PushID("Directional");
		if (ImGui::TreeNode("Directional Light")) {
			ImGui::ColorEdit4("Color", &directionalLightData_->color.x);
			ImGui::DragFloat3("Direction", &directionalLightData_->direction.x, 0.01f, -1.0f, 1.0f);
			ImGui::DragFloat("Intensity", &directionalLightData_->intensity, 0.01f, 0.0f, 10.0f);
			ImGui::TreePop();
		}
		ImGui::PopID();

		ImGui::PushID("Spot");
		if (ImGui::TreeNode("Spot Light")) {
			ImGui::ColorEdit4("Color", &spotLightData_->color.x);
			ImGui::DragFloat3("Position", &spotLightData_->position.x, 0.1f);
			ImGui::DragFloat3("Direction", &spotLightData_->direction.x, 0.01f);
			ImGui::DragFloat("Intensity", &spotLightData_->intensity, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("Distance", &spotLightData_->distance, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("Decay", &spotLightData_->decay, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("CosAngle", &spotLightData_->cosAngle, 0.001f, 0.0f, 1.0f);
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	ImGui::PopID();
}
#endif // _DEBUG

// 平行光源データ作成
void LightManager::CreateDirectionalLightData()
{
	// 平行光源用のリソースを作る
	directionalLightResource_ = dxBase_->CreateBufferResource(sizeof(MyBase::DirectionalLight));
	// 書き込むためのアドレス取得
	directionalLightResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	// デフォルト値はとりあえず以下のようにしておく
	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData_->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData_->intensity = 1.0f;
}

// 点光源データ作成
void LightManager::CreatePointLightData()
{
	// 点光源データ用のリソースを作る
	pointLightResource_ = dxBase_->CreateBufferResource(sizeof(MyBase::PointLight));
	// 書き込むためのアドレス取得
	pointLightResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
	pointLightData_->color = { 1.0f, 1.0f, 1.0f,1.0f };
	pointLightData_->position = { 0.0f, 0.0f, 0.0f };
	pointLightData_->intensity = 1.0f;
	pointLightData_->radius = 1.0f;
	pointLightData_->decay = 1.0f;
}

// スポットライトデータ作成
void LightManager::CreateSpotLightData()
{
	// スポットライトデータ用のリソースを作る
	spotLightResource_ = dxBase_->CreateBufferResource(sizeof(MyBase::SpotLight));
	// 書き込むためのアドレス取得
	spotLightResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));
	spotLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	spotLightData_->position = { 0.0f, 0.0f, 0.0f };
	spotLightData_->intensity = 1.0f;
	spotLightData_->direction = { 0.0f, -1.0f, 0.0f };
	spotLightData_->distance = 1.0f;
	spotLightData_->decay = 1.0f;
	spotLightData_->cosAngle = 0.125f;
}
