#include "Skydome.h"

Skydome::Skydome()
{
	
}

Skydome::~Skydome()
{
}

void Skydome::Initialize(MyBase::Vector3 position)
{
	object_ = std::make_unique<Object3d>();
	filePath_ = "resources/skydome.png";
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
