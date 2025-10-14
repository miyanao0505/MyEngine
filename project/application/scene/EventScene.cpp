#include "EventScene.h"
#include <imgui.h>
#include "CameraManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include"SceneManager.h"
#include "MyTools.h"

// 初期化
void EventScene::Initialize()
{
	BaseScene::Initialize();

#pragma region シーン初期化
	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(filePath1_);

	// スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(filePath1_);
	sprite_->SetPosition({ 0.0f, 0.0f });	// スプライトの位置を設定

	// .objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel("debug/cube", "cube.obj");

	// 3Dオブジェクト
	

	// パーティクル
	//particleEmitter_ = std::make_unique<ParticleEmitter>();
	//particleEmitter_->Initialize("circle", "resources/circle.png");
#pragma endregion シーン初期化

#pragma region 変数
	isParticleActive_ = true;
	//particleEmitter_->SetIsEmitUpdate(isParticleActive_);
	isAccelerationField_ = false;
	acceleration_ = { 15.0f, 0.0f, 0.0f };
	area_ = { .min{-1.0f, -1.0f, -1.0f}, .max{1.0f, 1.0f, 1.0f} };
#pragma endregion 変数
}

// 終了
void EventScene::Finalize()
{
	BaseScene::Finalize();

	// 3Dオブジェクト
	

	// スプライト
	sprite_.reset();
}

// 毎フレーム更新
void EventScene::Update()
{
	BaseScene::Update();

#ifdef _DEBUG
	// Nキーを押したら
	if (input_->TriggerKey(DIK_N)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	DebugDraw();
#endif // _DEBUG

	// 3Dオブジェクトの更新処理
	

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
	sprite_->Update();
}

// 描画
void EventScene::Draw()
{
#pragma region 3Dオブジェクト

	// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	ModelManager::GetInstance()->SetCommonScreen();

	// 全ての3DObject個々の描画
	

#pragma endregion 3Dオブジェクト

#pragma region パーティクル

	// パーティクルの描画準備。パーティクルの描画に共通グラフィックスコマンドを積む
	//ParticleManager::GetInstance()->Draw();

#pragma endregion パーティクル

#pragma region スプライト

	// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	TextureManager::GetInstance()->SetCommonScreen();

	// 全てのSprite個々の描画
	sprite_->Draw();

#pragma endregion スプライト

}

#ifdef _DEBUG
// デバッグ描画
void EventScene::DebugDraw()
{
	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::SetNextWindowPos(ImVec2(20, 350), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Event");
	ImGui::Text("N key : titleScene");
	ImGui::End();

	// デモウィンドウの表示オン
	//ImGui::ShowDemoWindow();
}
#endif // _DEBUG

// jsonファイルの読み込み
void EventScene::LoadJsonFile(const std::string& filePath)
{
	filePath;
}