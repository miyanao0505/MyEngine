#include "LevelData.h"
#include "ModelManager.h"

using namespace std;

bool LevelData::LoadFromFile(const std::string& jsonFilePath)
{
	auto levelObjects = JsonLoader::LoadFile(jsonFilePath);

	for (const auto& objData : levelObjects) {
		auto gameObject = std::make_shared<Object3d>();

		ModelManager::GetInstance()->LoadModel("resources/models/" + objData.meshName, objData.meshName + ".obj");

	}

	return false;
}

std::unique_ptr<Object3d>& LevelData::GetObjects(const std::string name) const
{
	// TODO: return ステートメントをここに挿入します
}
