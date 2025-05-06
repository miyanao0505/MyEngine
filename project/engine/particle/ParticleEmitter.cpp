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
	if (ImGui::CollapsingHeader("NewParticle")) {

	}
}

#endif // _DEBUG
