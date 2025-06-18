#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include <imgui.h>

ParticleEmitter::ParticleEmitter()
{
}

void ParticleEmitter::Initialize(const std::string name, const std::string textureFilePath, const ParticleType type)
{
	particleGroupNames_.push_back(name);
	textureFilePath_ = textureFilePath;

	particleSystem_ = std::make_unique<ParticleSystem>();
	particleSystem_->Initialize();
	particleSystem_->AddParticleGroupData(name);

	ParticleManager::GetInstance()->CreateIndexResource(type);

	if (type == Box) {
		ParticleManager::GetInstance()->CreateParticleGroup(name, textureFilePath_);
	}
	if (type == Ring) {
		ParticleManager::GetInstance()->CreateParticleGroupRing(name, textureFilePath_);
	}
	if (type == Cylinder) {
		ParticleManager::GetInstance()->CreateParticleGroupCylinder(name, textureFilePath_);
	}
}

void ParticleEmitter::Update()
{
	auto groupData = particleSystem_->GetParticleGroupData(particleGroupNames_[0]);

	// 登録されている分だけパーティクルを発生させる
	for (uint32_t i = 0; i < particleGroupNames_.size(); i++) {
		groupData = particleSystem_->GetParticleGroupData(particleGroupNames_[i]);
		if (groupData->isEmitUpdate) {
			groupData->frequencyTime -= kDeltaTime_;
			if (groupData->frequencyTime <= 0.0f) {
				groupData->frequencyTime = groupData->frequency;
				ParticleManager::GetInstance()->Emit(particleGroupNames_[i], transform_.translate, groupData->count);
			}
		}
	}
}

void ParticleEmitter::Emit()
{
	auto groupData = particleSystem_->GetParticleGroupData(particleGroupNames_[0]);

	// 登録されている分だけパーティクルを発生させる
	for (uint32_t i = 0; i < particleGroupNames_.size(); i++) {
		groupData = particleSystem_->GetParticleGroupData(particleGroupNames_[i]);
		ParticleManager::GetInstance()->Emit(particleGroupNames_[i], transform_.translate, groupData->count);
	}
}

#ifdef _DEBUG

void ParticleEmitter::Imgui(std::string name)
{
	std::string ID = name + "ParticleEmitter";
	ImGui::Begin(ID.c_str());
	{
		ImGui::PushID(ID.c_str());
		// 座標
		ImGui::DragFloat3("Translate", &transform_.translate.x, 0.1f);
		// 回転
		//ImGui::SliderAngle("particleEmitter_.Rotate", &transform_.rotate.x);
		// 拡縮
		ImGui::DragFloat3("Scale", &transform_.scale.x, 0.1f);
		particleSystem_->Imgui(name);
		// 発生させる
		if (ImGui::Button("Emit", { 100,50 })) {
			Emit();
		}
		// ビルボード設定
		bool isBillboard = GetIsBillboard(ID);
		ImGui::Checkbox("IsBillboard", &isBillboard);
		if (isBillboard != GetIsBillboard(ID)) {
			SetBillboard(ID, isBillboard);
		}
		ImGui::PopID();
	}
	ImGui::End();
}

#endif // _DEBUG

bool ParticleEmitter::GetIsBillboard(std::string name)
{
	return ParticleManager::GetInstance()->GetIsBillboard(name);
}

void ParticleEmitter::SetBillboard(std::string name, bool isBillboard)
{
	ParticleManager::GetInstance()->SetIsBillboard(name, isBillboard);
}