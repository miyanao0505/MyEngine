#pragma once
#include "BaseScene.h"
#include <string>
#include <vector>
#include "Sprite.h"
#include "Skybox.h"
#include "ParticleEmitter.h"
#include "MyBase.h"

// タイトルシーン
class TitleScene : public BaseScene
{
public:	// メンバ関数
	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;

	// デバッグ描画
	void DebugDraw() override;

	// jsonファイルの読み込み
	void LoadJsonFile(const std::string& filePath) override;

private:	// メンバ変数
#pragma region シーン初期化
	// テクスチャファイルパス
	std::string titleTextureFilePath_ = "Title.png";
	std::string skyBoxFilePath_ = "rostock_laage_airport_4k.dds";

	// スプライト
	std::unique_ptr<Sprite> titleSprite_ = nullptr;	// タイトルスプライト

	// 3Dオブジェクト
	// Skybox
	std::unique_ptr<Skybox> skybox_ = nullptr;

	// パーティクル
	//std::unique_ptr<ParticleEmitter> particleEmitter_ = nullptr;
#pragma endregion シーン初期化

	bool isParticleActive_;
	bool isAccelerationField_;
	MyBase::Vector3 acceleration_{};
	MyBase::AABB area_{};

	// デルタイム
	const float kDeltaTime_ = 1.0f / 60.0f;
};

