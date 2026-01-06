#pragma once
#include <map>
#include <random>
#include <numbers>
#include "ParticleBase.h"
#include "ParticleEmitter.h"
#include "MyBase.h"

using namespace std::numbers;

// 前方宣言
class DirectXBase;
class SrvManager;

/// <summary>
/// パーティクルマネージャークラス(ParticleManager)
/// ゲーム内で使用されるすべてのパーティクルグループを管理するシングルトンマネージャーです。
/// </summary>
class ParticleManager
{
public:	// パーティクルグループ構造体
	struct ParticleGroup {
		MyBase::MaterialData materialData;
		std::list<MyBase::Particle> particles;
		uint32_t numInstance;
		uint32_t srvIndexForInstancing;
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;			// バッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;		// バッファリソース
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;						// バッファリソースの使い道を補足するバッファビュー
		MyBase::ParticleVertexData* vertexData = nullptr;				// バッファリソース内のデータを指すポインタ
		MyBase::ParticleForGPU* instancingData = nullptr;				// バッファリソース内のデータを指すポインタ
		ParticleType type;
		bool isBillboard;	// ビルボードかどうか
	};

public:	// メンバ関数
	/// <summary>
	/// シングルトンインスタンスを取得
	/// </summary>
	/// <returns>ParticleManager のインスタンス</returns>
	static ParticleManager* GetInstance();
	
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(SrvManager* srvManager);

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
	/// ImGui
	/// </summary>
	void ImGui();
#endif // _DEBUG

	/// <summary>
	/// ブレンドモード変更
	/// </summary>
	/// <param name="blendMode">ブレンドモード</param>
	void ChangeBlendMode(BlendMode blendMode);

	/// <summary>
	/// パーティクルグループの生成
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="textureFilePath">テクスチャファイルパス</param>
	void CreateParticleGroup(const std::string& name, const std::string& textureFilePath);

	/// <summary>
	/// リング型パーティクルグループを生成
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="textureFilePath">テクスチャファイルパス</param>
	void CreateParticleGroupRing(const std::string& name, const std::string& textureFilePath);

	/// <summary>
	/// 円柱型パーティクルグループを生成
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="textureFilePath">テクスチャファイルパス</param>
	void CreateParticleGroupCylinder(const std::string& name, const std::string& textureFilePath);

	/// <summary>
	/// パーティクルの発生
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="position">位置</param>
	/// <param name="particleGroupData">パーティクルグループ毎のデータ</param>
	void Emit(const std::string& name, const MyBase::Vector3& position, const ParticleSystem::ParticleGroupData& particleGroupData);

	/// <summary>
	/// パーティクル描画で使用するインデックスバッファリソースを生成します。
	/// </summary>
	/// <param name="type">パーティクルタイプ(デフォルト: Ellipse)</param>
	void CreateIndexResource(ParticleType type = ParticleType::kEllipse);

public:	// getter
	/// <summary>
	/// すべてのパーティクルグループのマップを取得
	/// </summary>
	/// <returns>パーティクルグループのマップ</returns>
	std::map<std::string, std::unique_ptr<ParticleGroup>>& GetParticleGroups() { return particleGroups_; }

	/// <summary>
	/// 指定した名前のパーティクルグループを取得
	/// </summary>
	/// <param name="name">パーティクルグループ名</param>
	/// <returns>パーティクルグループ。存在しない場合は nullptr</returns>
	ParticleGroup* GetParticleGroup(const std::string& name);
	
	/// <summary>
	/// 指定したパーティクルグループがビルボードかどうかを返す
	/// </summary>
	/// <param name="name">パーティクルグループ名</param>
	bool GetIsBillboard(const std::string& name);

public:	// setter
	/// <summary>
	/// 指定したパーティクルグループにビルボード設定
	/// </summary>
	/// <param name="name">パーティクルグループ名</param>
	/// <param name="isBillboard">ビルボードを使用する場合 true</param>
	void SetIsBillboard(const std::string& name, bool isBillboard);

private: // ローカル関数
	/// <summary>
	/// パーティクルの生成
	/// </summary>
	/// <param name="randomEngine">乱数</param>
	/// <param name="translate">位置</param>
	/// <param name="particleGroupData">パーティクルグループ毎のデータ</param>
	/// <param name="type">パーティクルのタイプ</param>
	/// <returns>生成されたパーティクルデータ</returns>
	MyBase::Particle CreateParticle(std::mt19937& randomEngine, const MyBase::Vector3& translate, const ParticleSystem::ParticleGroupData& particleGroupData, ParticleType type = ParticleType::kEllipse);

private:	// シングルトン
	static ParticleManager* sInstance;

	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = delete;
	ParticleManager& operator=(ParticleManager&) = delete;

private:	// メンバ変数
	// ポインタ
	DirectXBase* dxBase_ = nullptr;
	SrvManager* srvManager_ = nullptr;
	std::unique_ptr<ParticleBase> particleBase_;

	// バッファリソースの使い道を遅くするバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;
	// バッファリソース内のデータを指すポインタ
	uint32_t* indexData_ = nullptr;

	uint32_t particleIndexSize_ = 0;	// インデックスリソースのサイズ	

	// パーティクル数
	uint32_t particleCount_ = 0;

	// テクスチャサイズ
	MyBase::Vector2 textureLeftTop_ = { 0.0f, 0.0f };
	MyBase::Vector2 textureSize_ = { 100.0f, 100.0f };

	// パーティクルデータ
	std::map<std::string, std::unique_ptr<ParticleGroup>> particleGroups_;

#pragma region 定数
	// Ellipse用
	static constexpr uint32_t kParticleVertexNum = 4;
	static constexpr float kEllipseScaleXRotio = 0.08f;	// Xスケール比率
	static constexpr uint32_t kQuadIndices[6] = { 0, 1, 2, 1, 3, 2 };
	// Ring用
	static constexpr uint32_t kRingDivide = 32;		// 分割数
	static constexpr float kOuterRadius = 1.0f;		// 外径
	static constexpr float kInnerRadius = 0.2f;		// 内径
	static constexpr float kRadianPerDivide = 2.0f * pi_v<float> / float(kRingDivide);	// 1つ分の角度(ラジアン)
	// Cylinder用
	static constexpr uint32_t kCylinderDivide = 32;		// 分割数
	static constexpr float kTopRadius = 1.0f;			// 上径
	static constexpr float kBottomRadius = 1.0f;		// 下径
	static constexpr float kHeight = 3.0f;				// 高さ
	static constexpr float kRadianPerDivideCylinder = 2.0f * pi_v<float> / float(kCylinderDivide);	// 1つ分の角度(ラジアン)
	static constexpr float kCylinderUVScrollRate = 0.05f;	// UVスクロール速度
	static constexpr float kCylinderDefaultLifeTime = 2.0f;	// デフォルトの寿命
	// 描画用
	static constexpr uint32_t kParticleIndexNum[3] = { 6, 6 * kRingDivide, 6 * kCylinderDivide };
	
	// インスタンスの最大数
	static constexpr uint32_t kMaxInstance_ = 1000;
	// デルタイム
	static constexpr float kDelTime = 1.0f / 60.0f;

	
#pragma endregion
};