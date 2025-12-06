#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "ParticleSystem.h"
#include <imgui.h>

ParticleEmitter::ParticleEmitter()
{
}

void ParticleEmitter::Initialize(const std::string& name, const std::string& textureFilePath, const ParticleType type)
{
	particleGroupNames_.push_back(name);

	particleSystem_ = std::make_unique<ParticleSystem>();
	particleSystem_->Initialize();

	CreateParticleGroup(name, textureFilePath, type);
}

void ParticleEmitter::Update()
{
	auto groupDataPtr = particleSystem_->GetParticleGroupData(particleGroupNames_[0]);

	// 登録されている分だけパーティクルを発生させる
	for (uint32_t i = 0; i < particleGroupNames_.size(); i++) {
		groupDataPtr = particleSystem_->GetParticleGroupData(particleGroupNames_[i]);
		if (groupDataPtr->isEmitUpdate) {
			groupDataPtr->frequencyTime -= kDeltaTime_;
			if (groupDataPtr->frequencyTime <= 0.0f) {
				groupDataPtr->frequencyTime = groupDataPtr->frequency;
				ParticleManager::GetInstance()->Emit(particleGroupNames_[i], transform_.translate, *groupDataPtr);
			}
		}
	}
}

void ParticleEmitter::Emit()
{
	auto groupDataPtr = particleSystem_->GetParticleGroupData(particleGroupNames_[0]);

	// 登録されている分だけパーティクルを発生させる
	for (uint32_t i = 0; i < particleGroupNames_.size(); i++) {
		groupDataPtr = particleSystem_->GetParticleGroupData(particleGroupNames_[i]);
		if (groupDataPtr) {
			ParticleManager::GetInstance()->Emit(particleGroupNames_[i], transform_.translate, *groupDataPtr);
		}
	}
}

void ParticleEmitter::CreateParticleGroup(const std::string& name, const std::string& textureFilePath, const ParticleType type)
{
	SetParticleGroupName(name);

	ParticleManager::GetInstance()->CreateIndexResource(type);

	if (type == ParticleType::kEllipse) {
		ParticleManager::GetInstance()->CreateParticleGroup(name, textureFilePath);
	}
	if (type == ParticleType::kRing) {
		ParticleManager::GetInstance()->CreateParticleGroupRing(name, textureFilePath);
	}
	if (type == ParticleType::kCylinder) {
		ParticleManager::GetInstance()->CreateParticleGroupCylinder(name, textureFilePath);
	}
}

#ifdef _DEBUG

void ParticleEmitter::ImGui(const std::string& name)
{
	std::string id = name + "ParticleEmitter";
	ImGui::Begin(id.c_str());
	{
		ImGui::PushID(id.c_str());
		// 座標
		ImGui::DragFloat3("Translate", &transform_.translate.x, 0.1f);
		// 回転
		//ImGui::SliderAngle("particleEmitter_.Rotate", &transform_.rotate.x);
		// 拡縮
		ImGui::DragFloat3("Scale", &transform_.scale.x, 0.1f);
		// 各パーティクルグループの設定を表示
		particleSystem_->ImGui(name);

		// 発生させる
		if (ImGui::Button("Emit", { 100,50 })) {
			Emit();
		}
		ImGui::PopID();
	}
	ImGui::End();
}

#endif // _DEBUG

/// getter
// パーティクルグループ毎のデータを取得
const ParticleSystem::ParticleGroupData& ParticleEmitter::GetParticleGroupData(const std::string& name)
{
	auto groupDataPtr = particleSystem_->GetParticleGroupData(name);
	if (groupDataPtr) {
		return *groupDataPtr; // データを返す
	}
	else {
		static ParticleSystem::ParticleGroupData emptyData; // 空のデータを返す
		return emptyData; // グループが存在しない場合は空のデータを返す
	}
}

bool ParticleEmitter::GetIsBillboard(const std::string& name)
{
	return ParticleManager::GetInstance()->GetIsBillboard(name);
}

/// setter
// パーティクルグループ名を設定
void ParticleEmitter::SetParticleGroupName(const std::string& name)
{
	// 既に存在するか確認
	auto it = std::find(particleGroupNames_.begin(), particleGroupNames_.end(), name);
	if (particleGroupNames_.empty() || it == particleGroupNames_.end()) {
		// 新しいパーティクルグループ名を追加
		particleGroupNames_.push_back(name);
		particleSystem_->SetParticleGroupName(name);
	}
	else {
		// 既存のパーティクルグループ名を更新
		particleSystem_->SetParticleGroupName(name);
	}
}

// パーティクルグループ毎のデータを設定
void ParticleEmitter::SetParticleGroupData(const std::string& name, ParticleSystem::ParticleGroupData& particleGroupData)
{
	// 新しいパーティクルグループデータを追加
	particleSystem_->SetParticleGroupData(name, particleGroupData);
}

void ParticleEmitter::SetBillboard(const std::string& name, bool isBillboard)
{
	ParticleManager::GetInstance()->SetIsBillboard(name, isBillboard);
}