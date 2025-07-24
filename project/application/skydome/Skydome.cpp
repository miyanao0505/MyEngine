#include "Skydome.h"
#include "ModelManager.h"
#include "TextureManager.h"

Skydome::Skydome()
{
	
}

Skydome::~Skydome()
{
}

void Skydome::Initialize(MyBase::Vector3 position)
{
	ModelManager::GetInstance()->LoadModel("debug/sphere", "sphere.obj");
	object_ = std::make_unique<Object3d>();

	filePath_ = "resources/texture/skydome.png";
	TextureManager::GetInstance()->LoadTexture(filePath_);

	object_->Initislize("sphere.obj");
	object_->SetTexture(filePath_);
	object_->SetScale({ 100.0f, 100.0f, 100.0f });
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
