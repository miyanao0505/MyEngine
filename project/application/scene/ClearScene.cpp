#include "ClearScene.h"
#include <imgui.h>
#include "CameraManager.h"
#include "LightManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "MyTools.h"

// 初期化
void ClearScene::Initialize()
{
#pragma region シーン初期化
	BaseScene::Initialize();

#pragma region カメラ
	CameraManager::GetInstance()->FindCamera("default");
	CameraManager::GetInstance()->GetCamera()->SetTranslate({ 0.0f, 0.0f, -40.0f });
	CameraManager::GetInstance()->GetCamera()->SetRotate({ 0.0f, 0.0f, 0.0f });
#pragma endregion カメラ

#pragma region ライト
	MyBase::PointLight pointLight;
	pointLight.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	pointLight.position = { 0.0f, 0.0f, -40.0f };
	pointLight.intensity = 1.0f;
	pointLight.radius = 500.0f;
	pointLight.decay = 2.0f;
	LightManager::GetInstance()->SetPointLight(pointLight);
#pragma endregion ライト

#pragma region スプライト
	// テクスチャの読み込み

	// スプライト

#pragma endregion スプライト

#pragma region 3Dオブジェクト
	// 3Dオブジェクト
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeFilePath_, { 0.0f, 0.0f, 0.0f }, { 100.0f, 100.0f, 100.0f });

	// ロゴ
	clearLogo_ = std::make_unique<ClearLogo>();
	clearLogo_->Initialize();
#pragma endregion 3Dオブジェクト

#pragma region パーティクル
	// パーティクル
	/*particleEmitter_ = std::make_unique<ParticleEmitter>();
	particleEmitter_->Initialize("circle", "resources/circle.png");*/
#pragma endregion パーティクル
	
#pragma region 変数
	isParticleActive_ = true;
	//particleEmitter_->SetIsEmitUpdate(isParticleActive_);
	isAccelerationField_ = false;
	acceleration_ = { 15.0f, 0.0f, 0.0f };
	area_ = { .min{-1.0f, -1.0f, -1.0f}, .max{1.0f, 1.0f, 1.0f} };
#pragma endregion 変数

#pragma endregion シーン初期化

	// 最初の更新
	CameraManager::GetInstance()->GetCamera()->Update();
	skydome_->Update();
	clearLogo_->Update();
}

// 終了
void ClearScene::Finalize()
{
	BaseScene::Finalize();

	// 3Dオブジェクト
	clearLogo_.reset();
	skydome_.reset();

	// スプライト
}

// 毎フレーム更新
void ClearScene::Update()
{
	BaseScene::Update();

#ifdef _DEBUG
	DebugUpdate();
#endif // _DEBUG

	// ゲームシーンへの遷移
	if (input_->TriggerKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
		return;
	}

	// カメラの更新
	CameraManager::GetInstance()->GetCamera()->Update();

	// 3Dオブジェクトの更新処理
	// 天球の更新
	skydome_->Update();

	// ロゴ
	clearLogo_->Update();

	if (isAccelerationField_) {
		for (std::pair<const std::string, std::unique_ptr<ParticleManager::ParticleGroup>>& pair : ParticleManager::GetInstance()->GetParticleGroups()) {
			ParticleManager::ParticleGroup& group = *pair.second;
			int index = 0;
			for (std::list<MyBase::Particle>::iterator it = group.particles.begin(); it != group.particles.end();) {
				MyBase::Particle& particle = *it;

				if (MyTools::IsCollision(area_, particle.transform.translate)) {
					particle.velocity = MyTools::Add(particle.velocity, MyTools::Multiply(kDeltaTime_, acceleration_));
				}

				++it;
				++index;
			}
		}
	}

	// パーティクルの更新処理
	//particleEmitter_->Update();
	ParticleManager::GetInstance()->Update();

	// スプライトの更新処理
}

// 描画
void ClearScene::Draw()
{
#pragma region 3Dオブジェクト

	// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	ModelManager::GetInstance()->SetCommonScreen();

	// 全ての3DObject個々の描画
	// 天球の描画
	skydome_->Draw();

	// ロゴ
	clearLogo_->Draw();

#pragma endregion 3Dオブジェクト

#pragma region パーティクル

	// パーティクルの描画準備。パーティクルの描画に共通グラフィックスコマンドを積む
	//ParticleManager::GetInstance()->Draw();

#pragma endregion パーティクル

#pragma region スプライト

	// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	TextureManager::GetInstance()->SetCommonScreen();

	// 全てのSprite個々の描画
	

#pragma endregion スプライト
}

#ifdef _DEBUG
/// デバッグ更新
void ClearScene::DebugUpdate()
{
	// Nキーを押したら
	if (input_->TriggerKey(DIK_N)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	// Bキーを押したら
	if (input_->TriggerKey(DIK_B)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("EVENT");
	}

	DebugDraw();
}

// デバッグ描画
void ClearScene::DebugDraw()
{
	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::SetNextWindowPos(ImVec2(20, 350), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Clear");
	ImGui::Text("N key : titleScene");
	ImGui::Text("B key : eventScene");
	ImGui::End();

	// デモウィンドウの表示オン
	//ImGui::ShowDemoWindow();
}
#endif // _DEBUG

// jsonファイルの読み込み
void ClearScene::LoadJsonFile(const std::string& filePath)
{
	filePath;
}
