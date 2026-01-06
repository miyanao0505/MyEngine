#pragma once
#include <vector>
#include <map>
#include <memory>
#include "MyBase.h"

// 前方宣言
class ParticleEmitter;

/// <summary>
/// パーティクルシステムクラス(ParticleSystem)
/// 複数のパーティクルグループを管理し、個々のパーティクルの発生・更新・描画を統括するクラスです。
/// </summary>
class ParticleSystem
{
private:
#pragma region 定数
	// 構造体の初期値
	static constexpr MyBase::ScopeF kDefaultSize = { 1.0f, 1.0f };
	static constexpr MyBase::ScopeF kDefaultEnergy = { 1.0f, 1.0f };
	static constexpr MyBase::ScopeI kDefaultCount = { 1, 1 };
	static constexpr MyBase::ScopeF kDefaultSpeed = { 1.0f, 1.0f };
	static constexpr MyBase::Vector3 kDefaultDirection = { 0.0f, 0.0f, 0.0f };
	static constexpr MyBase::Vector4 kDefaultColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	static constexpr float kDefaultFrequency = 1.5f;
	static constexpr float kDefaultFrequencyTime = 0.0f;
	static constexpr bool kDefaultIsBillboard = false;
	static constexpr bool kDefaultIsEmitUpdate = true;

#ifdef _DEBUG
	static constexpr float kImGuiDragSpeed = 0.01f;
	static constexpr int kImGuiCountDragSpeed = 1;
	static constexpr MyBase::ScopeF kSize = { 0.0f, 10.0f };
	static constexpr MyBase::ScopeF kEnergy = { 0.0f, 10.0f };
	static constexpr MyBase::ScopeI kParticleCount = { 1, 100 };
	static constexpr MyBase::ScopeF kSpeed = { 0.0f, 10.0f };
	static constexpr MyBase::ScopeF kDirection = { -1.0f, 1.0f };
	static constexpr MyBase::ScopeF kFrequency = { 0.1f, 5.0f };
#endif // _DEBUG

#pragma endregion

public: // 構造体
	struct ParticleGroupData {
		MyBase::ScopeF size = kDefaultSize;						// パーティクルのサイズ
		MyBase::ScopeF energy = kDefaultEnergy;					// パーティクルの寿命
		MyBase::ScopeI count = kDefaultCount;					// パーティクルの発生数
		MyBase::ScopeF speed = kDefaultSpeed;					// パーティクルの速度
		MyBase::Vector3 direction = kDefaultDirection;			// パーティクルの方向
		MyBase::Vector4 color = kDefaultColor;					// パーティクルの色
		float frequency = kDefaultFrequency;					// 発生頻度
		float frequencyTime = kDefaultFrequencyTime;			// 頻度用時刻
		bool isBillboard = kDefaultIsBillboard;					// ビルボードかどうか
		bool isEmitUpdate = kDefaultIsEmitUpdate;				// 更新時に発生するか
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
	/// <summary>
	/// ImGuiを使用したデバッグ表示
	/// </summary>
	/// <param name="name">対象パーティクルグループ名</param>
	void ImGui(const std::string& name);
#endif // _DEBUG

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
	/// パーティクルグループ名の設定
	/// </summary>
	/// <param name="groupName">設定するパーティクルグループ名</param>
	void SetParticleGroupName(const std::string& groupName);

	/// <summary>
	/// パーティクルグループデータの追加
	/// </summary>
	/// <param name="groupName">パーティクルグループ名</param>
	/// <param name="data">パーティクルグループデータ</param>
	void SetParticleGroupData(const std::string& groupName, const ParticleGroupData& data);

private: // メンバ変数
	std::map<std::string, std::unique_ptr<ParticleGroupData>> particleGroupDatas_;		// パーティクルグループ名とパーティクルデータのリスト
};