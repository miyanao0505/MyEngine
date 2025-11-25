#pragma once
#include "BaseScene.h"
#include <string>
#include <vector>
#include "Sprite.h"
#include "BaseObject.h"
#include "ParticleEmitter.h"
#include "MyBase.h"

/// <summary>
/// ゲーム中のイベント演出を管理するシーン。
/// </summary>
class EventScene : public BaseScene
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

#ifdef _DEBUG
	/// <summary>
	/// デバッグ描画
	/// </summary>
	void DebugDraw() override;
#endif // _DEBUG

	/// <summary>
	/// jsonファイルの読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadJsonFile(const std::string& filePath) override;

private:	// メンバ変数
	// テクスチャファイルパス
	std::string filePath1_ = "resources/texture/uvChecker.png";

	// スプライト
	std::unique_ptr<Sprite> sprite_;

	// モデルファイルパス
	

	// 3Dオブジェクト
	

	// パーティクル
	//std::unique_ptr<ParticleEmitter> particleEmitter_ = nullptr;

	bool isParticleActive_;
	bool isAccelerationField_;
	MyBase::Vector3 acceleration_{};
	MyBase::AABB area_{};

	// デルタイム
	const float kDeltaTime_ = 1.0f / 60.0f;
};

