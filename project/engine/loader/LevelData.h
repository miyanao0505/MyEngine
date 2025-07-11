#pragma once
#include <map>
#include <string>
#include <memory>
#include "BaseObject.h"
#include "MyBase.h"

class LevelData
{
public:	// メンバ関数
	bool LoadFromFile(const std::string& jsonFilePath);
	std::unique_ptr<BaseObject>& GetObjects(const std::string name) const;
	

public:	// getter

public:	// setter

private:	// メンバ変数
	std::map<std::string, std::unique_ptr<BaseObject>> objects_;	// オブジェクト名と3Dオブジェクトのマップ
};

