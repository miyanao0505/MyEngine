#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include <imgui.h>

ParticleEmitter::ParticleEmitter()
{
}

void ParticleEmitter::Initialize(const std::string name, const std::string textureFilePath, const ParticleType type)
{
	name_ = name;
	textureFilePath_ = textureFilePath;

	ParticleManager::GetInstance()->CreateIndexResource(type);

	if (type == Box) {
		ParticleManager::GetInstance()->CreateParticleGroup(name_, textureFilePath_);
	}
	if (type == Ring) {
		ParticleManager::GetInstance()->CreateParticleGroupRing(name_, textureFilePath_);
	}
}

void ParticleEmitter::Update()
{
	if (!isEmitUpdate_) { return; }
	frequencyTime_ -= kDeltaTime_;
	if (frequencyTime_ <= 0.0f) {
		frequencyTime_ = frequency_;
		ParticleManager::GetInstance()->Emit(name_, transform_.translate, count_);
	}
}

void ParticleEmitter::Emit()
{
	ParticleManager::GetInstance()->Emit(name_, transform_.translate, count_);
}

#ifdef _DEBUG

void ParticleEmitter::Imgui()
{
	ImGui::Begin(name_.c_str());
	{
		ImGui::PushID(name_.c_str());
		// 座標
		ImGui::DragFloat3("particleEmitter_.Translate", &transform_.translate.x, 0.1f);
		// 回転
		//ImGui::SliderAngle("particleEmitter_.Rotate", &transform_.rotate.x);
		// 拡縮
		ImGui::DragFloat3("particleEmitter_.Scale", &transform_.scale.x, 0.1f);
		// 発生数
		int count = count_;
		ImGui::DragInt("particleEmitter_.count", &count, 1, 0, 1000);
		count_ = count;
		// 発生頻度
		ImGui::DragFloat("particleEmitter_.frequency", &frequency_, 0.1f);
		// 発生させる
		if (ImGui::Button("ParticleEmit", { 100,50 })) {
			Emit();
		}
		// 連続発生
		ImGui::Checkbox("IsEmitUpdate", &isEmitUpdate_);
		ImGui::PopID();
	}
	ImGui::End();
}

#endif // _DEBUG
