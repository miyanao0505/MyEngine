#pragma once
#include "BaseScene.h"
#include <string>
#include <vector>
#include "Sprite.h"
#include "Skybox.h"
#include "Player.h"
#include "Enemy.h"
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

#ifdef _DEBUG
	// デバッグ描画
	void DebugDraw() override;
#endif // _DEBUG

	// jsonファイルの読み込み
	void LoadJsonFile(const std::string& filePath) override;

private:	// メンバ変数
#pragma region シーン初期化
	// テクスチャファイルパス
	std::string titleTextureFilePath_ = "resources/texture/Title.png";
	std::string skyBoxFilePath_ = "resources/texture/rostock_laage_airport_4k.dds";

	// スプライト
	std::unique_ptr<Sprite> titleSprite_ = nullptr;	// タイトルスプライト

	// Skybox
	std::unique_ptr<Skybox> skybox_ = nullptr;

	// 3Dオブジェクト
	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	// 敵
	std::unique_ptr<Enemy> enemy_ = nullptr;

	// test
	std::vector<std::unique_ptr<BaseObject>> testObjects_;

	// jsonローダー
	std::unique_ptr<JsonLoader> jsonLoader_ = nullptr;

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

