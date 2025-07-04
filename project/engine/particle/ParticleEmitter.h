#pragma once
#include "MyBase.h"
#include "ParticleSystem.h"

// パーティクルエミッター
class ParticleEmitter
{
public:	// メンバ関数
	enum ParticleType {
		Ellipse,	// 楕円
		Ring,		// リング
		Cylinder,	// 円柱
	};

	// コンストラクタ
	ParticleEmitter();

	// 初期化
	void Initialize(const std::string name, const std::string textureFilePath, const ParticleType type = Ellipse);

	// 更新
	void Update();

	// パーティクルの発生
	void Emit();

#ifdef _DEBUG
	void Imgui(std::string name);
#endif // _DEBUG

public:	// getter
	const MyBase::Vector3& GetPosition() { return transform_.translate; }
	const MyBase::Vector3& GetRotation() { return transform_.rotate; }
	const MyBase::Vector3& GetSize() { return transform_.scale; }
	const ParticleSystem::ParticleGroupData& GetParticleGroupData(std::string name);
	bool GetIsBillboard(std::string name);

public:	// setter
	void SetPosition(const MyBase::Vector3& position) { transform_.translate = position; }
	void SetRotation(const MyBase::Vector3& rotation) { transform_.rotate = rotation; }
	void SetSize(const MyBase::Vector3& size) { transform_.scale = size; }
	void SetParticleGroupData(const std::string& name, ParticleSystem::ParticleGroupData& particleGroupData);
	void SetBillboard(std::string name, bool isBillboard);

private:	// メンバ変数
	std::string textureFilePath_;

	// 発生中心
	MyBase::Transform transform_{};

	// パーティクルグループ名のリスト
	std::vector<std::string> particleGroupNames_;

	// パーティクルシステム
	std::unique_ptr<ParticleSystem> particleSystem_;

	// デルタタイム
	const float kDeltaTime_ = 1.0f / 60.0f;
};

