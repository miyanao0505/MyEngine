#include "BaseScene.h"
#include <string>
#include <filesystem>
#include "ModelManager.h"

using std::string;
using std::unique_ptr;
using std::make_unique;
namespace fs = std::filesystem;

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
unique_ptr<BaseObject> BaseScene::CreateObjectFromData(const JsonObjectData& data)
{
	auto baseObject = make_unique<BaseObject>();

	fs::path path(data.modelFileName);
	// ファイルパスからフォルダパスを抽出
	const string folderPath = path.parent_path().string();

	baseObject->Initialize("debug/" + folderPath, data.modelFileName);
	baseObject->SetName(data.name);
	baseObject->GetObject3D()->SetModel(data.modelFileName);
	baseObject->GetObject3D()->Initialize(data.modelFileName);
	Object3d* object = baseObject->GetObject3D();
	object->SetTranslate(data.translation);
	object->SetRotate(data.rotation);
	object->SetScale(data.scale);

	return baseObject;
}
