#include "BaseScene.h"
#include <string>
#include "ModelManager.h"

using namespace std;

void BaseScene::Initialize()
{
	input_ = Input::GetInstance();
}

void BaseScene::Finalize()
{
}

void BaseScene::Update()
{
}

void BaseScene::Draw()
{
}

#ifdef _DEBUG
void BaseScene::DebugDraw()
{
}
#endif // _DEBUG

// オブジェクト構築
BaseObject* BaseScene::CreateObjectFromData(const ObjectData& data)
{
	auto* baseObject = new BaseObject;
	size_t dotPos = data.objectName.find('.');
	const std::string folderPath = (dotPos != std::string::npos) ? data.objectName.substr(0, dotPos) : data.objectName;
	baseObject->Initialize("debug/" + folderPath, data.objectName);
	baseObject->SetName(data.name.c_str());
	baseObject->GetObject3d()->SetModel(data.objectName);
	baseObject->GetObject3d()->Initislize(data.objectName);
	Object3d* object = baseObject->GetObject3d();
	object->Initislize(data.objectName);
	object->SetTranslate(data.translation);
	object->SetRotate(data.rotation);
	object->SetScale(data.scale);

	return baseObject;
}
