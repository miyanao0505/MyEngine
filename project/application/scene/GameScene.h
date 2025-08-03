#pragma once
#include "BaseScene.h"
#include <string>
#include <vector>
#include "Sprite.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "ParticleEmitter.h"
#include "MyBase.h"

// ゲームプレイシーン
class GameScene : public BaseScene
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
	const std::string gameTextureFilePath_ = "resources/texture/Game.png";

	// スプライト
	std::unique_ptr<Sprite> gameSprite_ = nullptr;	// タイトルスプライト

	// モデルファイルパス
	

	// 3Dオブジェクト
	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	// 敵
	std::unique_ptr<Enemy> enemy_ = nullptr;
	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	
	// test
	//std::vector<std::unique_ptr<BaseObject>> testObjects_;

	// パーティクル

	// jsonローダー
	std::unique_ptr<JsonLoader> jsonLoader_ = nullptr;
	
#pragma endregion シーン初期化

	bool isParticleActive_;
	bool isAccelerationField_;
	MyBase::Vector3 acceleration_{};
	MyBase::AABB area_{};

	// デルタイム
	const float kDeltaTime_ = 1.0f / 60.0f;
};

