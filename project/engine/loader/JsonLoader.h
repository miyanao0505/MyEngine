#pragma once
#include <json.hpp>
#include <string>
#include <vector>
#include "MyBase.h"

struct ObjectData {
	std::string name;
	std::string meshName;
	std::string objectName;
	MyBase::Vector3 translation;
	MyBase::Vector3 rotation;
	MyBase::Vector3 scale;
	float radius;
	MyBase::AABB aabb;
	MyBase::OBB obb;
};
struct LevelData {
	std::vector<ObjectData> objects;
};

/// <summary>
/// Jsonファイルを読み込むクラス
/// </summary>
class JsonLoader
{
public:	// メンバ関数

	/// <summary>
	/// JSONファイルを読み込みLevelDataを返す
	/// </summary>
	/// <param name="filePath">読み込むJSONファイルのパス</param>
	/// <returns></returns>
	LevelData* LoadFile(const std::string& filePath);

private:	// メンバ関数
	void ParseObject(const nlohmann::json& object, LevelData& levelData);
};

