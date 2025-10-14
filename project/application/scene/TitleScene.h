#pragma once
#include "BaseScene.h"
#include <string>
#include <vector>
#include "Sprite.h"
#include "Skydome.h"
#include "TitleLogo.h"
#include "Characters.h"
#include "BaseObject.h"
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
	std::string skydomeFilePath_ = "resources/texture/skyback.png";

	// スプライト

	// Skydome
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// 3Dオブジェクト
	std::unique_ptr<TitleLogo> titleLogo_ = nullptr;
	std::unique_ptr<Characters> enterCharacters_ = nullptr;

	// jsonローダー
	std::unique_ptr<JsonLoader> jsonLoader_ = nullptr;

	// パーティクル
	
#pragma endregion シーン初期化

	
};

