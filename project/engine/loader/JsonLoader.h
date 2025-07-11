#pragma once
#include <json.hpp>
#include <string>
#include <vector>
#include "MyBase.h"

/// <summary>
/// Jsonファイルを読み込むクラス
/// </summary>
class JsonLoader
{
public:	// 構造体
	struct LevelObjectData {
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

public:	// メンバ関数

	/// <summary>
	/// JSONファイルを読み込む
	/// </summary>
	/// <param name="filePath">読み込むJSONファイルのパス</param>
	/// <returns></returns>
	static std::vector<LevelObjectData> LoadFile(const std::string& filePath);



public:	// getter
	/// <summary>
	/// 読み込んだJSONデータを取得する
	/// </summary>
	/// <returns>読み込んだJSONデータ</returns>
	const nlohmann::json& GetJsonData() const { return jsonData_; }

private:	// メンバ変数
	// JSONデータ
	nlohmann::json jsonData_;
};

