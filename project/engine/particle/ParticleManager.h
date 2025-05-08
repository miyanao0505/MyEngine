#pragma once
#include <map>
#include "ParticleBase.h"
#include "ParticleEmitter.h"
#include "random"
#include "numbers"
#include "MyBase.h"

// 前方宣言
class DirectXBase;
class SrvManager;

// パーティクルマネージャー
class ParticleManager
{
public:	// パーティクルグループ構造体
	struct ParticleGroup {
		MyBase::MaterialData materialData;
		std::list<MyBase::Particle> particles;
		uint32_t kNumInstance;
		uint32_t srvIndexForInstancing;
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;			// バッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;		// バッファリソース
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;						// バッファリソースの使い道を補足するバッファビュー
		MyBase::ParticleVertexData* vertexData = nullptr;				// バッファリソース内のデータを指すポインタ
		MyBase::ParticleForGPU* instancingData = nullptr;				// バッファリソース内のデータを指すポインタ
	};

public:	// メンバ関数
	// シングルトンインスタンスの取得
	static ParticleManager* GetInstance();
	// 終了
	void Finalize();

	// 初期化
	void Initialize(DirectXBase* dxBase, SrvManager* srvManager);

	// 更新
	void Update();

	// 描画
	void Draw();

#ifdef _DEBUG
	// Imgui
	void Imgui();
#endif // _DEBUG


	/// <summary>
	/// ブレンドモード変更
	/// </summary>
	/// <param name="blendMode">ブレンドモード</param>
	void ChangeBlendMode(ParticleBase::BlendMode blendMode);

	/// <summary>
	/// パーティクルグループの生成
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="textureFilePath">テクスチャファイルパス</param>
	void CreateParticleGroup(const std::string name, const std::string textureFilePath);

	/// <summary>
	/// パーティクルグループ(Ring)の生成
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="textureFilePath">テクスチャファイルパス</param>
	void CreateParticleGroupRing(const std::string name, const std::string textureFilePath);

	/// <summary>
	/// パーティクルの発生
	/// </summary>
	/// <param name="name"></param>
	/// <param name="position"></param>
	/// <param name="count"></param>
	void Emit(const std::string name, const MyBase::Vector3& position, uint32_t count);

	void CreateIndexResource(ParticleEmitter::ParticleType type = ParticleEmitter::Box);

public:	// getter
	std::map<std::string, std::unique_ptr<ParticleGroup>>& GetParticleGroups() { return particleGroups_; }

public:	// setter
	

private: // ローカル関数
	/// <summary>
	/// パーティクルの作成
	/// </summary>
	/// <param name="randomEngine"></param>
	/// <param name="position"></param>
	/// <returns></returns>
	MyBase::Particle CreateParticle(std::mt19937& randomEngine, const MyBase::Vector3& position);

	MyBase::Particle MakeNewParticle(std::mt19937& randomEngine, const MyBase::Vector3& translate);

private:	// シングルトン
	static ParticleManager* instance;

	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = delete;
	ParticleManager& operator=(ParticleManager&) = delete;

private:	// メンバ変数
	// ポインタ
	DirectXBase* dxBase_ = nullptr;
	SrvManager* srvManager_ = nullptr;
	std::unique_ptr<ParticleBase> particleBase_;

	// 定数
	const uint32_t kParticleVertexNum = 4;
	const uint32_t kParticleIndexNum = 6;
	// Ring用
	const uint32_t kRingDivide = 32;		// 分割数
	const float kOuterRadius = 1.0f;		// 外径
	const float kInnerRadius = 0.2f;		// 内径
	const float kRadianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kRingDivide);	// 1つ分の角度(ラジアン)

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;

	// バッファリソース内のデータを指すポインタ
	uint32_t* indexData_ = nullptr;

	// バッファリソースの使い道を遅くするバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	uint32_t particleIndexSize_ = 0;	// インデックスリソースのサイズ

	// インスタンスの最大数
	uint32_t kMaxInstance_ = 1000;

	// テクスチャサイズ
	MyBase::Vector2 textureLeftTop_ = { 0.0f, 0.0f };
	MyBase::Vector2 textureSize_ = { 100.0f, 100.0f };

	// デルタイム
	const float kDelTime_ = 1.0f / 60.0f;

	// パーティクルデータ
	std::map<std::string, std::unique_ptr<ParticleGroup>> particleGroups_;

};

