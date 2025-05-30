#pragma once
#include <vector>
#include <map>
#include <memory>
#include "MyBase.h"

class ParticleEmitter;

class particleSystem
{
public: // 構造体
	struct particleGroupData {
		std::string particleCroupName;							// パーティクルグループの名前
		MyBase::ScopeF size{ 1.0f, 1.0f };						// パーティクルのサイズ
		MyBase::ScopeF energy{ 1.0f, 1.0f };					// パーティクルの寿命
		MyBase::ScopeI count{ 1, 1 };							// パーティクルの発生数
		MyBase::Vector3 worldVelocity = { 0.0f, 0.0f, 0.0f };	// ワールド空間での速度
		MyBase::Vector3 localVelocity = { 0.0f, 0.0f, 0.0f };	// ローカル空間での速度
		MyBase::Vector3 rndomVelocity = { 0.0f, 0.0f, 0.0f };	// ランダムな速度
		float frequency = 1.5f;									// 発生頻度
		float frequencyTime = 0.0f;								// 頻度用時刻
		bool isEmitUpdate = true;								// 更新時に発生するか
	};

public:	// メンバ関数
	

public: // getter
	
public: // setter

private: // メンバ変数
	std::map<std::string, particleGroupData> particleGroupDataMap_;	// パーティクルグループのデータ
	std::map<std::unique_ptr<ParticleEmitter>, std::vector<std::string>> particleEmitters_;	// パーティクルエミッターとそのパーティクルグループの名前のマップ
	
};

