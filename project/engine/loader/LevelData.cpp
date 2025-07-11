#include "LevelData.h"
#include "JsonLoader.h"
#include "ModelManager.h"
#include "BaseObjectCollider.h"
#include "CollisionConfig.h"
#include "CollisionManager.h"

using namespace std;

bool LevelData::LoadFromFile(const string& jsonFilePath)
{
	auto levelObjects = JsonLoader::LoadFile(jsonFilePath);

	for (const auto& objData : levelObjects) {
		auto gameObject = make_unique<BaseObject>();

		if (objData.meshName.empty()) {
			// メッシュ名が空ならスキップ
			continue;
		}
		ModelManager::GetInstance()->LoadModel("resources/models/" + objData.meshName, objData.meshName + ".obj");

		// オブジェクトの初期化
		gameObject->Initialize(objData.meshName + ".obj");
		gameObject->GetObject3d()->SetTranslate(objData.translation);
		gameObject->GetObject3d()->SetRotate(objData.rotation);
		gameObject->GetObject3d()->SetScale(objData.scale);
		gameObject->GetObject3d()->SetTexture("resources/textures/" + objData.textureName + ".png");
		gameObject->GetObject3d()->SetEnableLighting(true); // ライティングを有効にする

		// コライダーを生成してGameObjectにセット
		auto collider = make_unique<BaseObjectCollider>(gameObject.get());
		collider->SetRadius(1.0f);
		

		// 衝突マネージャにコライダーを登録
		CollisionManager::GetInstance()->Register(collider.get());

		// マップに追加
		objects_[objData.meshName] = move(gameObject);
	}

	if (objects_.size() == 0) {
		// オブジェクトが一つも読み込まれなかった場合はfalseを返す
		return false;
	}

	return true;
}

unique_ptr<BaseObject>& LevelData::GetObjects(const string name) const
{
    return const_cast<unique_ptr<BaseObject>&>(objects_.at(name));
}
