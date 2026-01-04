#pragma once
#include <json.hpp>
#include <string>
#include <vector>
#include "MyBase.h"

/// <summary>
/// 3Dオブジェクトの情報を保持するデータ構造体。
/// </summary>
struct JsonObjectData {
	std::string name;			// オブジェクトの名前
	std::string colliderType;	// モデルメッシュ名
	std::string modelFileName;	// モデルファイル名
	MyBase::Vector3 translation;
	MyBase::Vector3 rotation;
	MyBase::Vector3 scale;
	float radius;
	MyBase::AABB aabb;
	MyBase::OBB obb;
	bool isDisabled = false;	// 無効化フラグ
	std::vector<JsonObjectData> children;
};
/// <summary>
/// ゲームシーン(レベル)に配置されるデータをまとめた構造体。
/// </summary>
struct JsonLevelData {
	std::vector<JsonObjectData> objects;				// シーン内のオブジェクトデータ
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
	std::unique_ptr<JsonLevelData> LoadFile(const std::string& filePath);

private:	// メンバ関数
	/// <summary>
	/// JSONオブジェクトからObjectDataを解析し、LevelDataへ登録
	/// 子オブジェクトが存在する場合は再帰的に解析
	/// </summary>
	/// <param name="object">解析対象のJSONオブジェクト</param>
	/// <param name="levelData">解析結果を格納するLevelData</param>
	/// <param name="outObject">親のObjectData (ルートの場合はnullptr)</param>
	void ParseObject(const nlohmann::json& object, JsonLevelData& levelData, JsonObjectData* outObject = nullptr);

#pragma region 定数
	
#pragma endregion
};