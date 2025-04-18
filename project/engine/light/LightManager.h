#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Object3dBase.h"
#include "CameraManager.h"


enum class LightType {
	Directional,
	Point,
	Spot,
};

// ライトマネージャー
class LightManager
{
public:	// メンバ関数
	// シングルトンインスタンスの取得
	static LightManager* GetInstance();
	// 終了
	void Finalize();

	// 初期化
	void Initialize(DirectXBase* dxBase);

	// 描画処理
	void Draw(Object3dBase* object3dBase);

private:	// メンバ関数
	// 平行光源データ作成
	void CreateDirectionalLightData();
	// 点光源データ作成
	void CreatePointLightData();
	// スポットライトデータ作成
	void CreateSpotLightData();

public:	// getter
	const MyBase::DirectionalLight& GetDirectionalLight() const { return *directionalLightData_; }
	const MyBase::PointLight& GetPointLight() const { return *pointLightData_; }
	const MyBase::SpotLight& GetSpotLight() const { return *spotLightData_; }
	const MyBase::Vector4& GetDirectionalLightColor() const { return directionalLightData_->color; }
	const MyBase::Vector3& GetDirectionalLightDirection() const { return directionalLightData_->direction; }
	const float& GetDirectionalLightIntensity() const { return directionalLightData_->intensity; }
	const MyBase::Vector4& GetPointLightColor() const { return pointLightData_->color; }
	const MyBase::Vector3& GetPointLightPosition() const { return pointLightData_->position; }
	const float& GetPointLightIntensity() const { return pointLightData_->intensity; }
	const float& GetPointLightRadius() const { return pointLightData_->radius; }
	const float& GetPointLightDecay() const { return pointLightData_->decay; }
	const MyBase::Vector4& GetSpotLightColor() const { return spotLightData_->color; }
	const MyBase::Vector3& GetSpotLightPosition() const { return spotLightData_->position; }
	const float& GetSpotLightIntensity() const { return spotLightData_->intensity; }
	const MyBase::Vector3& GetSpotLightDirection() const { return spotLightData_->direction; }
	const float& GetSpotLightDistance() const { return spotLightData_->distance; }
	const float& GetSpotLightDecay() const { return spotLightData_->decay; }
	const float& GetSpotLightCosAngle() const { return spotLightData_->cosAngle; }

public:	// setter
	void SetDirectionalLight(const MyBase::DirectionalLight& directionalLight) { *directionalLightData_ = directionalLight; }
	void SetPointLight(const MyBase::PointLight& pointLight) { *pointLightData_ = pointLight; }
	void SetSpotLight(const MyBase::SpotLight& spotLight) { *spotLightData_ = spotLight; }
	void SetDirectionalLightColor(const MyBase::Vector4& lightColor) { directionalLightData_->color = lightColor; }
	void SetDirectionalLightDirection(const MyBase::Vector3& lightDirection) { directionalLightData_->direction = lightDirection; }
	void SetDirectionalLightIntensity(const float& lightIntensity) { directionalLightData_->intensity = lightIntensity; }
	void SetPointLightColor(const MyBase::Vector4& lightColor) { pointLightData_->color = lightColor; }
	void SetPointLightPosition(const MyBase::Vector3& lightPosition) { pointLightData_->position = lightPosition; }
	void SetPointLightIntensity(const float& lightIntensity) { pointLightData_->intensity = lightIntensity; }
	void SetPointLightRadius(const float& lightRadius) { pointLightData_->radius = lightRadius; }
	void SetPointLightDecay(const float& lightDecay) { pointLightData_->decay = lightDecay; }
	void SetSpotLightColor(const MyBase::Vector4& lightColor) { spotLightData_->color = lightColor; }
	void SetSpotLightPosition(const MyBase::Vector3& lightPosition) { spotLightData_->position = lightPosition; }
	void SetSpotLightIntensity(const float& lightIntensity) { spotLightData_->intensity = lightIntensity; }
	void SetSpotLightDirection(const MyBase::Vector3& lightDirection) { spotLightData_->direction = lightDirection; }
	void SetSpotLightDistance(const float& lightDistance) { spotLightData_->distance = lightDistance; }
	void SetSpotLightDecay(const float& lightDecay) { spotLightData_->decay = lightDecay; }
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

