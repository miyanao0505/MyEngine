#include "JsonLoader.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace nlohmann;

vector<JsonLoader::LevelObjectData> JsonLoader::LoadFile(const string& filePath)
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

	// レベルデータ格納用
	vector<LevelObjectData> levelData;

	// "objects"の全オブジェクトを走査
	for (json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		string type = object["type"].get<string>();

		/// 種類ごとの処理
		// MESH
		if (type.compare("MESH") == 0) {
			// オブジェクトデータを生成
			LevelObjectData objectData;
			// 必要なデータを取得
			objectData.name = object["name"];
			objectData.meshName = object["mesh"];
			objectData.textureName = object["texture"];
			json& transform = object["transform"];
			// 平行移動
			objectData.translation = { (float)transform["translation"][0], (float)transform["translation"][2], (float)transform["translation"][1] };
			// 回転
			objectData.rotation = { -(float)transform["rotation"][0], -(float)transform["rotation"][2], -(float)transform["rotation"][1] };
			// 拡大縮小
			objectData.scale = { (float)transform["scale"][0], (float)transform["scale"][2], (float)transform["scale"][1] };
			// レベルデータに追加
			levelData.push_back(objectData);
		}

		/// 再帰処理
		if (object.contains("children")) {
			// 子オブジェクトがある場合は再帰的に処理
			for (json& child : object["children"]) {
				assert(child.contains("type"));
				// 子オブジェクトのタイプを取得
				string childType = child["type"].get<string>();
				// 子オブジェクトがMESHの場合は同様に処理
				if (childType.compare("MESH") == 0) {
					LevelObjectData childObjectData;
					childObjectData.name = child["name"];
					childObjectData.meshName = child["mesh"];
					childObjectData.textureName = child["texture"];
					json& transform = child["transform"];
					childObjectData.translation = { (float)transform["translation"][0], (float)transform["translation"][2], (float)transform["translation"][1] };
					childObjectData.rotation = { -(float)transform["rotation"][0], -(float)transform["rotation"][2], -(float)transform["rotation"][1] };
					childObjectData.scale = { (float)transform["scale"][0], (float)transform["scale"][2], (float)transform["scale"][1] };
					levelData.push_back(childObjectData);
				}
			}
		}

		return levelData;
	}
}
