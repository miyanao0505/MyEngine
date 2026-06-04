#include "JsonLoader.h"
#include <fstream>
#include <iostream>
#include <cassert>
#include "AssetPath.h"

using namespace std;
using namespace nlohmann;

namespace {
	constexpr int kAxisX = 0;
	constexpr int kAxisY = 1;
	constexpr int kAxisZ = 2;
}

namespace {
	constexpr const char* kTypeMesh = "MESH";
	constexpr const char* kTypePlayerSpawn = "PlayerSpawn";
}

namespace {
	constexpr const char* kColliderSphere = "SPHERE";
	constexpr const char* kColliderAABB = "AABB";
	constexpr const char* kColliderOBB = "OBB";
}

/// JSONファイルを読み込みレベルデータ(LevelData)を返す
std::unique_ptr<JsonLevelData> JsonLoader::LoadFile(const string& filePath)
{
	// 連結してフルパスを得る
	const string absolutePath = AssetPath::kJsonRootPath + filePath;

	// ファイルストリーム
	ifstream file;

	// ファイルを開く
	file.open(absolutePath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	string name = deserialized["name"].get<string>();
	// 正しいレベルデータファイル化チェック
	assert(name.compare("scene") == 0);

	// レベルデータを格納するマップ
	std::unique_ptr<JsonLevelData> levelData = std::make_unique<JsonLevelData>();

	// "objects"の全オブジェクトを走査
	for (json& object : deserialized["objects"]) {
		ParseObject(object, *levelData);
	}
	return levelData;
}

/// JSONオブジェクトからObjectDataを解析し、LevelDataへ登録
void JsonLoader::ParseObject(const nlohmann::json& object, JsonLevelData& levelData, JsonObjectData* outObject)
{
	assert(object.contains("type"));

	if (object.contains("disabled")) {
		// 有効無効フラグ
		bool disabled = object["disabled"].get<bool>();
		if (disabled) {
			// 配置しない
			return;
		}
	}

	// 種別を取得
	string type = object["type"].get<string>();

	/// 種類ごとの処理
	// MESH
	if (type.compare(kTypeMesh) == 0) {
		// オブジェクトデータを生成
		JsonObjectData objectData;
		// 必要なデータを取得
		objectData.name = object["name"];
		json transform = object["transform"];
		// 平行移動
		objectData.translation = { (float)transform["translation"][kAxisX], (float)transform["translation"][kAxisZ], (float)transform["translation"][kAxisY] };
		// 回転
		objectData.rotation = { -(float)transform["rotation"][0], -(float)transform["rotation"][2], -(float)transform["rotation"][1] };
		// 拡大縮小
		objectData.scale = { (float)transform["scaling"][0], (float)transform["scaling"][2], (float)transform["scaling"][1] };
		// オブジェクト名があれば取得
		if (object.contains("file_name")) {
			objectData.modelFileName = object["file_name"];
		}
		// コライダーがあれば取得
		if (object.contains("collider")) {
			json collider = object["collider"];
			objectData.colliderType = collider["type"];
			if (objectData.colliderType.compare(kColliderSphere) == 0) {
				objectData.radius = (float)collider["radius"];
			}
			else if (objectData.colliderType.compare(kColliderAABB) == 0) {
				// AABBコライダーの場合
				json aabb = collider["aabb"];
				objectData.aabb.min = { (float)aabb["min"][0], (float)aabb["min"][2], (float)aabb["min"][1] };
				objectData.aabb.max = { (float)aabb["max"][0], (float)aabb["max"][2], (float)aabb["max"][1] };
			}
			else if (objectData.colliderType.compare(kColliderOBB) == 0) {
				// OBBコライダーの場合
				json obb = collider["obb"];
				objectData.obb.center = { (float)obb["center"][0], (float)obb["center"][2], (float)obb["center"][1] };
				objectData.obb.size = { (float)obb["size"][0], (float)obb["size"][2], (float)obb["size"][1] };
			}
		}

		// 子オブジェクトがあれば再帰的に処理
		if (object.contains("children")) {
			for (const auto& child : object["children"]) {
				JsonObjectData childData;
				ParseObject(child, levelData, &childData);
				objectData.children.push_back(childData);
			}
		}

		if (outObject) {
			*outObject = objectData;
		}
		else {
			// レベルデータに追加
			levelData.objects.push_back(objectData);
		}
	}
	// 自キャラ生成ポイント
	else if (type.compare(kTypePlayerSpawn) == 0) {
		MyBase::PlayerSpawnData playerSpawn;
		// 必要なデータを取得
		json transform = object["transform"];
		// 平行移動
		playerSpawn.translation = { (float)transform["translation"][0], (float)transform["translation"][2], (float)transform["translation"][1] };
		// 回転
		playerSpawn.rotation = { -(float)transform["rotation"][0], -(float)transform["rotation"][2], -(float)transform["rotation"][1] };
		levelData.players.push_back(playerSpawn);
	}
}
