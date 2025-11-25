#pragma once
#include <json.hpp>
#include <string>
#include <vector>
#include "MyBase.h"

/// <summary>
/// 3Dオブジェクトの情報を保持するデータ構造体。
/// </summary>
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
	bool isDisabled = false;	// 無効化フラグ
	std::vector<ObjectData> children;
};
/// <summary>
/// ゲームシーン(レベル)に配置されるデータをまとめた構造体。
/// </summary>
struct LevelData {
	std::vector<ObjectData> objects;				// シーン内のオブジェクトデータ
	std::vector<MyBase::PlayerSpawnData> players;	// プレイヤー生成データ
};

/// <summary>
/// Jsonファイルを読み込むクラス
/// </summary>
class JsonLoader
{
public:	// メンバ関数

	/// <summary>
	/// JSONファイルを読み込みレベルデータ(LevelData)を返す
	/// </summary>
	/// <param name="filePath">読み込むJSONファイルのパス</param>
	/// <returns>読み込んだデータを格納したLevelData構造体のポインタ</returns>
	LevelData* LoadFile(const std::string& filePath);

private:	// メンバ関数
	/// <summary>
	/// JSONオブジェクトからObjectDataを解析し、LevelDataへ登録
	/// 子オブジェクトが存在する場合は再帰的に解析
	/// </summary>
	/// <param name="object">解析対象のJSONオブジェクト</param>
	/// <param name="levelData">解析結果を格納するLevelData</param>
	/// <param name="parent">親のObjectData (ルートの場合はnullptr)</param>
	void ParseObject(const nlohmann::json& object, LevelData& levelData, ObjectData* parent = nullptr);
};

