#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include <imgui.h>

// 初期化
void ParticleSystem::Initialize()
{
}

// 更新
void ParticleSystem::Update()
{
}

// 描画
void ParticleSystem::Draw()
{
}

#ifdef _DEBUG
void ParticleSystem::ImGui(const std::string& name)
{
	std::string ID = name;
	for (auto& [groupName, groupData] : particleGroupDatas_) {
		ID = name.c_str() + groupName;
		ImGui::PushID(ID.c_str());
		if (ImGui::CollapsingHeader(ID.c_str())) {
			ImGui::Text("Group Name: %s", groupName.c_str());
			ImGui::DragFloat2("Size", &groupData->size.min, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat2("Energy", &groupData->energy.min, 0.01f, 0.0f, 10.0f);
			ImGui::DragInt2("Count", &groupData->count.min, 1, 1, 100);
			ImGui::DragFloat2("Speed", &groupData->speed.min, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat3("Direction", &groupData->direction.x, 0.01f, -1.0f, 1.0f);
			ImGui::ColorEdit4("Color", &groupData->color.x);
			ImGui::DragFloat("Frequency", &groupData->frequency, 0.01f, 0.1f, 5.0f);
			ImGui::Checkbox("IsBillboard", &groupData->isBillboard);
			ImGui::Checkbox("IsEmitUpdate", &groupData->isEmitUpdate);
		}
		ImGui::PopID();
	}
}
#endif // _DEBUG

// パーティクルグループ名の設定
void ParticleSystem::SetParticleGroupName(const std::string& groupName)
{
	if (particleGroupDatas_.empty() || particleGroupDatas_.count(groupName) == 0) {
		// 新しいパーティクルグループ名を追加
		particleGroupDatas_[groupName] = std::make_unique<ParticleGroupData>();
	}
}

// パーティクルグループデータの追加
void ParticleSystem::SetParticleGroupData(const std::string& groupName, const ParticleGroupData& data)
{
	if (!particleGroupDatas_.empty()) {
		// パーティクルグループデータのセット
		particleGroupDatas_[groupName] = std::make_unique<ParticleGroupData>(data);
	}
}