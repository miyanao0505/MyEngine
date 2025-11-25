#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Object3dBase.h"

/// <summary>
/// 3Dシーン内で使用されるライトの種類を表す列挙型。
/// </summary>
enum class LightType {
	Directional,
	Point,
	Spot,
};

/// <summary>
/// 3Dシーン内のライト(平行光源・点光源・スポットライト)を管理するシングルトンクラス。
/// </summary>
class LightManager
{
public:	// メンバ関数
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>LightManager のインスタンス</returns>
	static LightManager* GetInstance();

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
	/// <param name="object3dBase">描画対象となる Object3dBase</param>
	void Draw(Object3dBase* object3dBase);

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
	const MyBase::DirectionalLight& GetDirectionalLight() const { return *directionalLightData_; }
	
	/// <summary>
	/// 点光源データの取得
	/// </summary>
	/// <returns>点光源データ</returns>
	const MyBase::PointLight& GetPointLight() const { return *pointLightData_; }
	
	/// <summary>
	/// スポットライトデータの取得
	/// </summary>
	/// <returns>スポットライトデータ</returns>
	const MyBase::SpotLight& GetSpotLight() const { return *spotLightData_; }
	
	/// <summary>
	/// 平行光源の色の取得
	/// </summary>
	/// <returns>平行光源の色</returns>
	const MyBase::Vector4& GetDirectionalLightColor() const { return directionalLightData_->color; }
	
	/// <summary>
	/// 平行光源の方向の取得
	/// </summary>
	/// <returns>平行光源の方向</returns>
	const MyBase::Vector3& GetDirectionalLightDirection() const { return directionalLightData_->direction; }
	
	/// <summary>
	/// 平行光源の強度の取得
	/// </summary>
	/// <returns>平行光源の強度</returns>
	const float& GetDirectionalLightIntensity() const { return directionalLightData_->intensity; }
	
	/// <summary>
	/// 点光源の色の取得
	/// </summary>
	/// <returns>点光源の色</returns>
	const MyBase::Vector4& GetPointLightColor() const { return pointLightData_->color; }
	
	/// <summary>
	/// 点光源の位置の取得
	/// </summary>
	/// <returns>点光源の位置</returns>
	const MyBase::Vector3& GetPointLightPosition() const { return pointLightData_->position; }
	
	/// <summary>
	/// 点光源の強度の取得
	/// </summary>
	/// <returns>点光源の強度</returns>
	const float& GetPointLightIntensity() const { return pointLightData_->intensity; }
	
	/// <summary>
	/// 点光源の届く最大距離の取得
	/// </summary>
	/// <returns>点光源の届く最大距離</returns>
	const float& GetPointLightRadius() const { return pointLightData_->radius; }
	
	/// <summary>
	/// 点光源の減衰率の取得
	/// </summary>
	/// <returns>点光源の減衰率</returns>
	const float& GetPointLightDecay() const { return pointLightData_->decay; }
	
	/// <summary>
	/// スポットライトの色の取得
	/// </summary>
	/// <returns>スポットライトの色</returns>
	const MyBase::Vector4& GetSpotLightColor() const { return spotLightData_->color; }
	
	/// <summary>
	/// スポットライトの位置の取得
	/// </summary>
	/// <returns>スポットライトの位置</returns>
	const MyBase::Vector3& GetSpotLightPosition() const { return spotLightData_->position; }
	
	/// <summary>
	/// スポットライトの強度の取得
	/// </summary>
	/// <returns>スポットライトの強度</returns>
	const float& GetSpotLightIntensity() const { return spotLightData_->intensity; }
	
	/// <summary>
	/// スポットライトの方向の取得
	/// </summary>
	/// <returns>スポットライトの方向</returns>
	const MyBase::Vector3& GetSpotLightDirection() const { return spotLightData_->direction; }
	
	/// <summary>
	/// スポットライトの届く最大距離の取得
	/// </summary>
	/// <returns>スポットライトの届く最大距離</returns>
	const float& GetSpotLightDistance() const { return spotLightData_->distance; }
	
	/// <summary>
	/// スポットライトの減衰率の取得
	/// </summary>
	/// <returns>スポットライトの減衰率</returns>
	const float& GetSpotLightDecay() const { return spotLightData_->decay; }
	
	/// <summary>
	/// スポットライトの余弦の取得
	/// </summary>
	/// <returns>スポットライトの余弦</returns>
	const float& GetSpotLightCosAngle() const { return spotLightData_->cosAngle; }

public:	// setter
	/// <summary>
	/// 平行光源データの設定
	/// </summary>
	/// <param name="directionalLight">平行光源データ</param>
	void SetDirectionalLight(const MyBase::DirectionalLight& directionalLight) { *directionalLightData_ = directionalLight; }
	
	/// <summary>
	/// 点光源データの設定
	/// </summary>
	/// <param name="pointLight">点光源データ</param>
	void SetPointLight(const MyBase::PointLight& pointLight) { *pointLightData_ = pointLight; }
	
	/// <summary>
	/// スポットライトデータの設定
	/// </summary>
	/// <param name="spotLight">スポットライトデータ</param>
	void SetSpotLight(const MyBase::SpotLight& spotLight) { *spotLightData_ = spotLight; }
	
	/// <summary>
	/// 平行光源の色の設定
	/// </summary>
	/// <param name="lightColor">平行光源の色</param>
	void SetDirectionalLightColor(const MyBase::Vector4& lightColor) { directionalLightData_->color = lightColor; }
	
	/// <summary>
	/// 平行光源の方向の設定
	/// </summary>
	/// <param name="lightDirection">平行光源の方向</param>
	void SetDirectionalLightDirection(const MyBase::Vector3& lightDirection) { directionalLightData_->direction = lightDirection; }
	
	/// <summary>
	/// 平行光源の強度の設定
	/// </summary>
	/// <param name="lightIntensity">平行光源の強度</param>
	void SetDirectionalLightIntensity(const float& lightIntensity) { directionalLightData_->intensity = lightIntensity; }
	
	/// <summary>
	/// 点光源の色の設定
	/// </summary>
	/// <param name="lightColor">点光源の色</param>
	void SetPointLightColor(const MyBase::Vector4& lightColor) { pointLightData_->color = lightColor; }
	
	/// <summary>
	/// 点光源の位置の設定
	/// </summary>
	/// <param name="lightPosition">点光源の位置</param>
	void SetPointLightPosition(const MyBase::Vector3& lightPosition) { pointLightData_->position = lightPosition; }
	
	/// <summary>
	/// 点光源の強度の設定
	/// </summary>
	/// <param name="lightIntensity">点光源の強度</param>
	void SetPointLightIntensity(const float& lightIntensity) { pointLightData_->intensity = lightIntensity; }
	
	/// <summary>
	/// 点光源の届く最大距離の設定
	/// </summary>
	/// <param name="lightRadius">点光源の届く最大距離</param>
	void SetPointLightRadius(const float& lightRadius) { pointLightData_->radius = lightRadius; }
	
	/// <summary>
	/// 点光源の減衰率の設定
	/// </summary>
	/// <param name="lightDecay">点光源の減衰率</param>
	void SetPointLightDecay(const float& lightDecay) { pointLightData_->decay = lightDecay; }
	
	/// <summary>
	/// スポットライトの色の設定
	/// </summary>
	/// <param name="lightColor">スポットライトの色</param>
	void SetSpotLightColor(const MyBase::Vector4& lightColor) { spotLightData_->color = lightColor; }
	
	/// <summary>
	/// スポットライトの位置の設定
	/// </summary>
	/// <param name="lightPosition">スポットライトの位置</param>
	void SetSpotLightPosition(const MyBase::Vector3& lightPosition) { spotLightData_->position = lightPosition; }
	
	/// <summary>
	/// スポットライトの強度の設定
	/// </summary>
	/// <param name="lightIntensity">スポットライトの強度</param>
	void SetSpotLightIntensity(const float& lightIntensity) { spotLightData_->intensity = lightIntensity; }
	
	/// <summary>
	/// スポットライトの方向の設定
	/// </summary>
	/// <param name="lightDirection">スポットライトの方向</param>
	void SetSpotLightDirection(const MyBase::Vector3& lightDirection) { spotLightData_->direction = lightDirection; }
	
	/// <summary>
	/// スポットライトの届く最大距離の設定
	/// </summary>
	/// <param name="lightDistance">スポットライトの届く最大距離</param>
	void SetSpotLightDistance(const float& lightDistance) { spotLightData_->distance = lightDistance; }
	
	/// <summary>
	/// スポットライトの減衰率の設定
	/// </summary>
	/// <param name="lightDecay">スポットライトの減衰率</param>
	void SetSpotLightDecay(const float& lightDecay) { spotLightData_->decay = lightDecay; }
	
	/// <summary>
	/// スポットライトの余弦の設定
	/// </summary>
	/// <param name="lightCosAngle">スポットライトの余弦</param>
	void SetSpotLightCosAngle(const float& lightCosAngle) { spotLightData_->cosAngle = lightCosAngle; }

private:	// シングルトンインスタンス
	static LightManager* instance;

	LightManager() = default;
	~LightManager() = default;
	LightManager(LightManager&) = default;
	LightManager& operator=(LightManager&) = delete;

private:	// メンバ変数
	// DirectXBase
	DirectXBase* dxBase_ = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;		// 平行光源
	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource_ = nullptr;			// 点光源
	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource_ = nullptr;			// スポットライト

	// バッファリソース内のデータを指すポインタ
	MyBase::DirectionalLight* directionalLightData_ = nullptr;
	MyBase::PointLight* pointLightData_ = nullptr;									// 点光源
	MyBase::SpotLight* spotLightData_ = nullptr;

};

