#include "JsonLoader.h"
#include <fstream>
#include <iostream>
#include <cassert>

using namespace std;
using namespace nlohmann;

LevelData* JsonLoader::LoadFile(const string& filePath)
{
	// 連結してフルパスを得る
	const string fullpath = "resources/jsons/" + filePath;

	// ファイルストリーム
	ifstream file;

	// ファイルを開く
	file.open(fullpath);
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
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (json& object : deserialized["objects"]) {
		ParseObject(object, *levelData);
	}
	return levelData;
}

void JsonLoader::ParseObject(const nlohmann::json& object, LevelData& levelData, ObjectData* parent)
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
	if (type.compare("MESH") == 0) {
		// オブジェクトデータを生成
		ObjectData objectData;
		// 必要なデータを取得
		objectData.name = object["name"];
		json transform = object["transform"];
		// 平行移動
		objectData.translation = { (float)transform["translation"][0], (float)transform["translation"][2], (float)transform["translation"][1] };
		// 回転
		objectData.rotation = { -(float)transform["rotation"][0], -(float)transform["rotation"][2], -(float)transform["rotation"][1] };
		// 拡大縮小
		objectData.scale = { (float)transform["scaling"][0], (float)transform["scaling"][2], (float)transform["scaling"][1] };
		// オブジェクト名があれば取得
		if (object.contains("file_name")) {
			objectData.objectName = object["file_name"];
		}
		// コライダーがあれば取得
		if (object.contains("collider")) {
			json collider = object["collider"];
			objectData.meshName = collider["type"];
			if (objectData.meshName.compare("SPHERE") == 0) {
				objectData.radius = (float)collider["radius"];
			}
			else if (objectData.meshName.compare("AABB") == 0) {
				// AABBコライダーの場合
				json aabb = object["Collider"]["aabb"];
				objectData.aabb.min = { (float)aabb["min"][0], (float)aabb["min"][2], (float)aabb["min"][1] };
				objectData.aabb.max = { (float)aabb["max"][0], (float)aabb["max"][2], (float)aabb["max"][1] };
			}
			else if (objectData.meshName.compare("OBB") == 0) {
				// OBBコライダーの場合
				json obb = object["Collider"]["obb"];
				objectData.obb.center = { (float)obb["center"][0], (float)obb["center"][2], (float)obb["center"][1] };
				objectData.obb.size = { (float)obb["size"][0], (float)obb["size"][2], (float)obb["size"][1] };
			}
		}

		// 子オブジェクトがあれば再帰的に処理
		if (object.contains("children")) {
			for (const auto& child : object["children"]) {
				ObjectData childData;
				ParseObject(child, levelData, &childData);
				objectData.children.push_back(childData);
			}
		}

		if (parent) {
			*parent = objectData;
		}
		else {
			// レベルデータに追加
			levelData.objects.push_back(objectData);
		}
	}
}
