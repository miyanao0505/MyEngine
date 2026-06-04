#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Object3dBase.h"

/// <summary>
/// 3Dシーン内で使用されるライトの種類を表す列挙型
/// </summary>
enum class LightType {
	Directional,
	Point,
	Spot,
};

/// <summary>
/// ルートパラメータのインデックスを表す列挙型
/// </summary>
enum RootParameterIndex {
	kDirectionalLight = 3,
	kPointLight = 5,
	kSpotLight = 6,
};

/// <summary>
/// 3Dシーン内のライト(平行光源・点光源・スポットライト)を管理するマネージャークラス。
/// </summary>
class LightManager
{
public:	// メンバ関数
	/// <summary>
	/// Singleton Instance を取得
	/// </summary>
	/// <returns>LightManager</returns>
	static LightManager* GetInstance();

	/// ------ Passkey Idion ------
	/// コントラクタを渡すための鍵
	class ConstructorKey {
	private:
		ConstructorKey() = default;
		friend class LightManager;
	};

	/// PassKeyを受け取るコンストラクタ
	explicit LightManager(ConstructorKey) {}

	/// コピー禁止
	LightManager(const LightManager&) = delete;
	LightManager& operator=(const LightManager&) = delete;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmd">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmd);

#ifdef _DEBUG
	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw();
#endif // _DEBUG

private:	// メンバ関数
	/// <summary>
	/// 平行光源データ作成
	/// </summary>
	void CreateDirectionalLightData();

	/// <summary>
	/// 点光源データ作成
	/// </summary>
	void CreatePointLightData();

	/// <summary>
	/// スポットライトデータ作成
	/// </summary>
	void CreateSpotLightData();

public:	// getter
	/// <summary>
	/// 平行光源データの取得
	/// </summary>
	/// <returns>平行光源データ</returns>
	const MyBase::DirectionalLight& GetDirectionalLight() const { return *directionalLightMapped_; }
	
	/// <summary>
	/// 点光源データの取得
	/// </summary>
	/// <returns>点光源データ</returns>
	const MyBase::PointLight& GetPointLight() const { return *pointLightMapped_; }
	
	/// <summary>
	/// スポットライトデータの取得
	/// </summary>
	/// <returns>スポットライトデータ</returns>
	const MyBase::SpotLight& GetSpotLight() const { return *spotLightMapped_; }
	
	/// <summary>
	/// 平行光源の色の取得
	/// </summary>
	/// <returns>平行光源の色</returns>
	const MyBase::Vector4& GetDirectionalLightColor() const { return directionalLightMapped_->color; }
	
	/// <summary>
	/// 平行光源の方向の取得
	/// </summary>
	/// <returns>平行光源の方向</returns>
	const MyBase::Vector3& GetDirectionalLightDirection() const { return directionalLightMapped_->direction; }
	
	/// <summary>
	/// 平行光源の強度の取得
	/// </summary>
	/// <returns>平行光源の強度</returns>
	const float& GetDirectionalLightIntensity() const { return directionalLightMapped_->intensity; }
	
	/// <summary>
	/// 点光源の色の取得
	/// </summary>
	/// <returns>点光源の色</returns>
	const MyBase::Vector4& GetPointLightColor() const { return pointLightMapped_->color; }
	
	/// <summary>
	/// 点光源の位置の取得
	/// </summary>
	/// <returns>点光源の位置</returns>
	const MyBase::Vector3& GetPointLightPosition() const { return pointLightMapped_->position; }
	
	/// <summary>
	/// 点光源の強度の取得
	/// </summary>
	/// <returns>点光源の強度</returns>
	const float& GetPointLightIntensity() const { return pointLightMapped_->intensity; }
	
	/// <summary>
	/// 点光源の届く最大距離の取得
	/// </summary>
	/// <returns>点光源の届く最大距離</returns>
	const float& GetPointLightRadius() const { return pointLightMapped_->radius; }
	
	/// <summary>
	/// 点光源の減衰率の取得
	/// </summary>
	/// <returns>点光源の減衰率</returns>
	const float& GetPointLightDecay() const { return pointLightMapped_->decay; }
	
	/// <summary>
	/// スポットライトの色の取得
	/// </summary>
	/// <returns>スポットライトの色</returns>
	const MyBase::Vector4& GetSpotLightColor() const { return spotLightMapped_->color; }
	
	/// <summary>
	/// スポットライトの位置の取得
	/// </summary>
	/// <returns>スポットライトの位置</returns>
	const MyBase::Vector3& GetSpotLightPosition() const { return spotLightMapped_->position; }
	
	/// <summary>
	/// スポットライトの強度の取得
	/// </summary>
	/// <returns>スポットライトの強度</returns>
	const float& GetSpotLightIntensity() const { return spotLightMapped_->intensity; }
	
	/// <summary>
	/// スポットライトの方向の取得
	/// </summary>
	/// <returns>スポットライトの方向</returns>
	const MyBase::Vector3& GetSpotLightDirection() const { return spotLightMapped_->direction; }
	
	/// <summary>
	/// スポットライトの届く最大距離の取得
	/// </summary>
	/// <returns>スポットライトの届く最大距離</returns>
	const float& GetSpotLightDistance() const { return spotLightMapped_->distance; }
	
	/// <summary>
	/// スポットライトの減衰率の取得
	/// </summary>
	/// <returns>スポットライトの減衰率</returns>
	const float& GetSpotLightDecay() const { return spotLightMapped_->decay; }
	
	/// <summary>
	/// スポットライトの余弦の取得
	/// </summary>
	/// <returns>スポットライトの余弦</returns>
	const float& GetSpotLightCosAngle() const { return spotLightMapped_->cosAngle; }

public:	// setter
	/// <summary>
	/// 平行光源データの設定
	/// </summary>
	/// <param name="directionalLight">平行光源データ</param>
	void SetDirectionalLight(const MyBase::DirectionalLight& directionalLight) { *directionalLightMapped_ = directionalLight; }
	
	/// <summary>
	/// 点光源データの設定
	/// </summary>
	/// <param name="pointLight">点光源データ</param>
	void SetPointLight(const MyBase::PointLight& pointLight) { *pointLightMapped_ = pointLight; }
	
	/// <summary>
	/// スポットライトデータの設定
	/// </summary>
	/// <param name="spotLight">スポットライトデータ</param>
	void SetSpotLight(const MyBase::SpotLight& spotLight) { *spotLightMapped_ = spotLight; }
	
	/// <summary>
	/// 平行光源の色の設定
	/// </summary>
	/// <param name="lightColor">平行光源の色</param>
	void SetDirectionalLightColor(const MyBase::Vector4& lightColor) { directionalLightMapped_->color = lightColor; }
	
	/// <summary>
	/// 平行光源の方向の設定
	/// </summary>
	/// <param name="lightDirection">平行光源の方向</param>
	void SetDirectionalLightDirection(const MyBase::Vector3& lightDirection) { directionalLightMapped_->direction = lightDirection; }
	
	/// <summary>
	/// 平行光源の強度の設定
	/// </summary>
	/// <param name="lightIntensity">平行光源の強度</param>
	void SetDirectionalLightIntensity(float lightIntensity) { directionalLightMapped_->intensity = lightIntensity; }
	
	/// <summary>
	/// 点光源の色の設定
	/// </summary>
	/// <param name="lightColor">点光源の色</param>
	void SetPointLightColor(const MyBase::Vector4& lightColor) { pointLightMapped_->color = lightColor; }
	
	/// <summary>
	/// 点光源の位置の設定
	/// </summary>
	/// <param name="lightPosition">点光源の位置</param>
	void SetPointLightPosition(const MyBase::Vector3& lightPosition) { pointLightMapped_->position = lightPosition; }
	
	/// <summary>
	/// 点光源の強度の設定
	/// </summary>
	/// <param name="lightIntensity">点光源の強度</param>
	void SetPointLightIntensity(float lightIntensity) { pointLightMapped_->intensity = lightIntensity; }
	
	/// <summary>
	/// 点光源の届く最大距離の設定
	/// </summary>
	/// <param name="lightRadius">点光源の届く最大距離</param>
	void SetPointLightRadius(float lightRadius) { pointLightMapped_->radius = lightRadius; }
	
	/// <summary>
	/// 点光源の減衰率の設定
	/// </summary>
	/// <param name="lightDecay">点光源の減衰率</param>
	void SetPointLightDecay(float lightDecay) { pointLightMapped_->decay = lightDecay; }
	
	/// <summary>
	/// スポットライトの色の設定
	/// </summary>
	/// <param name="lightColor">スポットライトの色</param>
	void SetSpotLightColor(const MyBase::Vector4& lightColor) { spotLightMapped_->color = lightColor; }
	
	/// <summary>
	/// スポットライトの位置の設定
	/// </summary>
	/// <param name="lightPosition">スポットライトの位置</param>
	void SetSpotLightPosition(const MyBase::Vector3& lightPosition) { spotLightMapped_->position = lightPosition; }
	
	/// <summary>
	/// スポットライトの強度の設定
	/// </summary>
	/// <param name="lightIntensity">スポットライトの強度</param>
	void SetSpotLightIntensity(float lightIntensity) { spotLightMapped_->intensity = lightIntensity; }
	
	/// <summary>
	/// スポットライトの方向の設定
	/// </summary>
	/// <param name="lightDirection">スポットライトの方向</param>
	void SetSpotLightDirection(const MyBase::Vector3& lightDirection) { spotLightMapped_->direction = lightDirection; }
	
	/// <summary>
	/// スポットライトの届く最大距離の設定
	/// </summary>
	/// <param name="lightDistance">スポットライトの届く最大距離</param>
	void SetSpotLightDistance(float lightDistance) { spotLightMapped_->distance = lightDistance; }
	
	/// <summary>
	/// スポットライトの減衰率の設定
	/// </summary>
	/// <param name="lightDecay">スポットライトの減衰率</param>
	void SetSpotLightDecay(float lightDecay) { spotLightMapped_->decay = lightDecay; }
	
	/// <summary>
	/// スポットライトの余弦の設定
	/// </summary>
	/// <param name="lightCosAngle">スポットライトの余弦</param>
	void SetSpotLightCosAngle(float lightCosAngle) { spotLightMapped_->cosAngle = lightCosAngle; }

private:	// Singleton Instance
	static std::unique_ptr<LightManager> sInstance_;

private:	// メンバ変数
	// DirectXBase
	DirectXBase* dxBase_ = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightBuffer_ = nullptr;	// 平行光源
	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightBuffer_ = nullptr;			// 点光源
	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightBuffer_ = nullptr;			// スポットライト

	// バッファリソース内のデータを指すポインタ
	MyBase::DirectionalLight* directionalLightMapped_ = nullptr;				// 平行光源
	MyBase::PointLight* pointLightMapped_ = nullptr;							// 点光源
	MyBase::SpotLight* spotLightMapped_ = nullptr;								// スポットライト

#pragma region 定数
	static constexpr float kDefaultSpotLightCosAngle = 0.125f;

#ifdef _DEBUG
	static constexpr MyBase::ScopeF kLightIntensity = { 0.0f, 10.0f };
	static constexpr float kLightIntensitySpeed = 0.01f;
#endif // _DEBUG
#pragma endregion
};