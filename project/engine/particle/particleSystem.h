#pragma once
#include <vector>
#include <map>
#include <memory>
#include "MyBase.h"

class ParticleEmitter;

class ParticleSystem
{
public: // 構造体
	struct ParticleGroupData {
		MyBase::ScopeF size{ 1.0f, 1.0f };						// パーティクルのサイズ
		MyBase::ScopeF energy{ 1.0f, 1.0f };					// パーティクルの寿命
		MyBase::ScopeI count{ 1, 1 };							// パーティクルの発生数
		MyBase::ScopeF speed{ 1.0f, 1.0f };						// パーティクルの速度
		MyBase::Vector3 direction = { 0.0f, 0.0f, 0.0f };		// パーティクルの方向
		MyBase::Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };		// パーティクルの色
		float frequency = 1.5f;									// 発生頻度
		float frequencyTime = 0.0f;								// 頻度用時刻
		bool isBillboard = false;								// ビルボードかどうか
		bool isEmitUpdate = true;								// 更新時に発生するか
	};

public:	// メンバ関数
	ParticleSystem() = default;
	~ParticleSystem() = default;
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

#ifdef _DEBUG
	void Imgui(std::string name);
#endif // _DEBUG

	/// <summary>
	/// パーティクルグループデータの追加
	/// </summary>
	/// <param name="groupName">パーティクルグループ名</param>
	void AddParticleGroupData(const std::string& groupName);

public: // getter
	/// <summary>
	/// パーティクルグループデータの取得
	/// </summary>
	/// <returns>パーティクルグループデータのマップ</returns>
	const std::map<std::string, std::unique_ptr<ParticleGroupData>>& GetParticleGroupDatas() { return particleGroupDatas_; }


	/// <summary>
	/// パーティクルグループデータの取得
	/// </summary>
	/// <param name="groupName">パーティクルグループ名</param>
	/// <returns>パーティクルグループデータ</returns>
	ParticleGroupData* GetParticleGroupData(const std::string& groupName) {
		auto it = particleGroupDatas_.find(groupName);
		if (it != particleGroupDatas_.end()) {
			return it->second.get();
		}
		return nullptr;
	}	

public: // setter
	/// <summary>
	/// パーティクルグループデータの設定
	/// </summary>
	/// <param name="groupName">パーティクルグループ名</param>
	/// <param name="data">パーティクルグループデータ</param>
	void SetParticleGroupData(const std::string& groupName, std::unique_ptr<ParticleGroupData> data) {
		particleGroupDatas_[groupName] = std::move(data);
	}

private: // メンバ変数
	std::map<std::string, std::unique_ptr<ParticleGroupData>> particleGroupDatas_;		// パーティクルグループ名とパーティクルデータのリスト
	
};

