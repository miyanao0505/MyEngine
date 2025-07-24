#pragma once
#include "BaseScene.h"
#include <string>
#include <vector>
#include "Sprite.h"
#include "Object3d.h"
#include "ParticleEmitter.h"
#include "MyBase.h"

// タイトルシーン
class ClearScene : public BaseScene
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
	std::string clearTextureFilePath_ = "resources/texture/Clear.png";

	// スプライト
	std::unique_ptr<Sprite> clearSprite_ = nullptr;	// クリアスプライト

	// モデルファイルパス
	

	// 3Dオブジェクト
	

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

