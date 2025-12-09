#include "LightManager.h"
#include "ModelManager.h"
#include "CameraManager.h"
#include "imgui.h"

LightManager* LightManager::sInstance = nullptr;

// シングルトンインスタンスの取得
LightManager* LightManager::GetInstance()
{
	if (sInstance == nullptr) {
		sInstance = new LightManager;
	}
	return sInstance;
}

// 終了
void LightManager::Finalize()
{
	if (directionalLightBuffer_) {
		directionalLightBuffer_.Reset();
	}
	if (pointLightBuffer_) {
		pointLightBuffer_.Reset();
	}
	if (spotLightBuffer_) {
		spotLightBuffer_.Reset();
	}

	delete sInstance;
	sInstance = nullptr;
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
void LightManager::Draw(ID3D12GraphicsCommandList* cmd)
{
	// RootParameter index (Engine 固定)
	// 3 : Directional Light(平行光源用)のCBufferの場所を設定
	cmd->SetGraphicsRootConstantBufferView(3, directionalLightBuffer_.Get()->GetGPUVirtualAddress());
	// 5 : Point Light(点光源用)のCBufferの場所を設定
	cmd->SetGraphicsRootConstantBufferView(5, pointLightBuffer_.Get()->GetGPUVirtualAddress());
	// 6 : Spot Light(スポットライト用)のCBufferの場所を設定
	cmd->SetGraphicsRootConstantBufferView(6, spotLightBuffer_.Get()->GetGPUVirtualAddress());
}

#ifdef _DEBUG
// デバッグ描画
void LightManager::DebugDraw()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Light")) {
		ImGui::PushID("Point");
		if (ImGui::TreeNode("Point Light")) {
			ImGui::ColorEdit4("Color", &pointLightMapped_->color.x);
			ImGui::DragFloat3("Position", &pointLightMapped_->position.x, 0.1f);
			ImGui::DragFloat("Intensity", &pointLightMapped_->intensity, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("Radius", &pointLightMapped_->radius, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("Decay", &pointLightMapped_->decay, 0.01f, 0.0f, 10.0f);
			ImGui::TreePop();
		}
		ImGui::PopID();

		ImGui::PushID("Directional");
		if (ImGui::TreeNode("Directional Light")) {
			ImGui::ColorEdit4("Color", &directionalLightMapped_->color.x);
			ImGui::DragFloat3("Direction", &directionalLightMapped_->direction.x, 0.01f, -1.0f, 1.0f);
			ImGui::DragFloat("Intensity", &directionalLightMapped_->intensity, 0.01f, 0.0f, 10.0f);
			ImGui::TreePop();
		}
		ImGui::PopID();

		ImGui::PushID("Spot");
		if (ImGui::TreeNode("Spot Light")) {
			ImGui::ColorEdit4("Color", &spotLightMapped_->color.x);
			ImGui::DragFloat3("Position", &spotLightMapped_->position.x, 0.1f);
			ImGui::DragFloat3("Direction", &spotLightMapped_->direction.x, 0.01f);
			ImGui::DragFloat("Intensity", &spotLightMapped_->intensity, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("Distance", &spotLightMapped_->distance, 0.01f, 0.0f, 100.0f);
			ImGui::DragFloat("Decay", &spotLightMapped_->decay, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("CosAngle", &spotLightMapped_->cosAngle, 0.001f, 0.0f, 1.0f);
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
	directionalLightBuffer_ = dxBase_->CreateBufferResource(sizeof(MyBase::DirectionalLight));
	// Uploadバッファは Map したままで OK(頻繁な書き換え前提)
	directionalLightBuffer_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightMapped_));
	// デフォルト値はとりあえず以下のようにしておく
	directionalLightMapped_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightMapped_->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightMapped_->intensity = 1.0f;
}

// 点光源データ作成
void LightManager::CreatePointLightData()
{
	// 点光源データ用のリソースを作る
	pointLightBuffer_ = dxBase_->CreateBufferResource(sizeof(MyBase::PointLight));
	// Uploadバッファは Map したままで OK(頻繁な書き換え前提)
	pointLightBuffer_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&pointLightMapped_));
	// デフォルト値はとりあえず以下のようにしておく
	pointLightMapped_->color = { 1.0f, 1.0f, 1.0f,1.0f };
	pointLightMapped_->position = { 0.0f, 0.0f, 0.0f };
	pointLightMapped_->intensity = 1.0f;
	pointLightMapped_->radius = 1.0f;
	pointLightMapped_->decay = 1.0f;
}

// スポットライトデータ作成
void LightManager::CreateSpotLightData()
{
	// スポットライトデータ用のリソースを作る
	spotLightBuffer_ = dxBase_->CreateBufferResource(sizeof(MyBase::SpotLight));
	// Uploadバッファは Map したままで OK(頻繁な書き換え前提)
	spotLightBuffer_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&spotLightMapped_));
	// デフォルト値はとりあえず以下のようにしておく
	spotLightMapped_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	spotLightMapped_->position = { 0.0f, 0.0f, 0.0f };
	spotLightMapped_->intensity = 1.0f;
	spotLightMapped_->direction = { 0.0f, -1.0f, 0.0f };
	spotLightMapped_->distance = 1.0f;
	spotLightMapped_->decay = 1.0f;
	spotLightMapped_->cosAngle = 0.125f;
}
