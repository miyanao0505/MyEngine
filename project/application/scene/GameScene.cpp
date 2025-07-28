#include "GameScene.h"
#include <imgui.h>
#include "CameraManager.h"
#include "LightManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "AudioManager.h"
#include"SceneManager.h"
#include "MyTools.h"

using namespace std;

// 初期化
void GameScene::Initialize()
{
#pragma region シーン初期化
	BaseScene::Initialize();

#pragma region スプライト
	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(gameTextureFilePath_);

	// スプライト
	gameSprite_ = std::make_unique<Sprite>();
	gameSprite_->Initialize(gameTextureFilePath_);
	gameSprite_->SetPosition({ 0.0f, 0.0f });	// スプライトの位置を設定
#pragma endregion スプライト

#pragma region 3Dオブジェクト
	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize({ 0.0f, 0.0f, 0.0f });

	// 敵
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();

	// 天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize({ 0.0f, 0.0f, 0.0f });
#pragma endregion 3Dオブジェクト

#pragma region パーティクル
	// パーティクル
	
#pragma endregion パーティクル

#pragma region jsonローダー
	// jsonローダー
	jsonLoader_ = std::make_unique<JsonLoader>();
	LoadJsonFile("gameScene.json");
#pragma endregion jsonローダー

#pragma region オーディオ
	// BGM

	// お試し用
	AudioManager::GetInstance()->LoadAudioWave("audio/fanfare.wav");
#pragma endregion オーディオ

#pragma region 変数
	isParticleActive_ = true;
	isAccelerationField_ = false;
	acceleration_ = { 15.0f, 0.0f, 0.0f };
	area_ = { .min{-1.0f, -1.0f, -1.0f}, .max{1.0f, 1.0f, 1.0f} };
#pragma endregion 変数

#pragma endregion シーン初期化
}

// 終了
void GameScene::Finalize()
{
	BaseScene::Finalize();

	// 3Dオブジェクト
	/*for (auto& obj : testObjects_) {
		obj.reset();
	}*/
	
	// スプライト
	gameSprite_.reset();
}

// 毎フレーム更新
void GameScene::Update()
{
	BaseScene::Update();

#ifdef _DEBUG
	// Nキーを押したら
	if (input_->TriggerKey(DIK_N)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}
	// Mキーを押したら
	if (input_->TriggerKey(DIK_M)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}
	// Bキーを押したら
	if (input_->TriggerKey(DIK_B)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("EVENT");
	}
	
	// Pキーを押したら
	if (input_->TriggerKey(DIK_P)) {
		// パーティクル描画フラグのOn / Off
		//isParticleActive_ = particleEmitter_->GetIsEmitUpdate();
		isParticleActive_ = !isParticleActive_;
		//particleEmitter_->SetIsEmitUpdate(isParticleActive_);
	}
	// Oキーを押したら
	if (input_->TriggerKey(DIK_O)) {
		// アクセラレーションのOn / Off
		isAccelerationField_ = !isAccelerationField_;
	}

	// Kキーを押したら
	if (input_->TriggerKey(DIK_K)) {
		// お試しの音を鳴らす
		AudioManager::GetInstance()->PlayWave("audio/fanfare.wav");
	}
	// Uキーを押したら
	if (input_->TriggerKey(DIK_U)) {
		// お試しの音を解放する
		AudioManager::GetInstance()->UnLoadAudio("audio/fanfare.wav");
	}
	// Lキーを押したら
	if (input_->TriggerKey(DIK_L)) {
		// お試しの音をロードする
		AudioManager::GetInstance()->LoadAudioWave("audio/fanfare.wav");
	}

	DebugDraw();
#endif // _DEBUG

	// 3Dオブジェクトの更新処理
	// プレイヤーの更新処理
	player_->Update();

	// 敵の更新処理
	enemy_->Update();

	// 天球の更新
	skydome_->Update();

	// test
	/*for (auto& obj : testObjects_) {
		obj->Update();
	}*/

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
	ParticleManager::GetInstance()->Update();

	// スプライトの更新処理
	gameSprite_->Update();
}

// 描画
void GameScene::Draw()
{
#pragma region 3Dオブジェクト

	// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	ModelManager::GetInstance()->SetCommonScreen();

	// 全ての3DObject個々の描画
	// 天球の描画
	skydome_->Draw();

	// 敵の描画
	enemy_->Draw();

	// プレイヤーの描画
	player_->Draw();

	// test
	/*for (auto& obj : testObjects_) {
		obj->Draw();
	}*/

#pragma endregion 3Dオブジェクト

#pragma region パーティクル

	// パーティクルの描画準備。パーティクルの描画に共通グラフィックスコマンドを積む
	ParticleManager::GetInstance()->Draw();

#pragma endregion パーティクル

#pragma region スプライト

	// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	TextureManager::GetInstance()->SetCommonScreen();

	// 全てのSprite個々の描画
	gameSprite_->Draw();

#pragma endregion スプライト
}

void GameScene::DebugDraw()
{
	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::SetNextWindowPos(ImVec2(20, 350), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Game");
	ImGui::Text("N key : clearScene");
	ImGui::Text("M key : gameOverScene");
	ImGui::Text("B key : eventScene");
	ImGui::Text("\n");
	ImGui::Text("P key : particle On / Off");
	ImGui::Text("O key : acceleration On / Off");
	ImGui::Text("\n");
	ImGui::Text("K key : sampleAudio Play");
	ImGui::Text("U key : sampleAudio UnLoad");
	ImGui::Text("L key : sampleAudio Load");
	ImGui::End();

	// デモウィンドウの表示オン
	//ImGui::ShowDemoWindow();
	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::SetNextWindowPos(ImVec2(900, 20), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Settings");
	// カメラ
	if (ImGui::CollapsingHeader("Camera"))
	{
		// 変更するための変数
		MyBase::Transform transformCamera{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

		static ImGuiComboFlags flags = 0;
		const char* cameraNames[] = { "default", "sub" };
		static int cameraIndex = 0;

		const char* cameraNowVlue = cameraNames[cameraIndex];

		if (ImGui::BeginCombo("Now Camera", cameraNowVlue, flags))
		{
			for (int i = 0; i < IM_ARRAYSIZE(cameraNames); i++)
			{
				const bool isSelected = (cameraIndex == i);
				if (ImGui::Selectable(cameraNames[i], isSelected)) {
					cameraIndex = i;
					CameraManager::GetInstance()->FindCamera(cameraNames[i]);
				}

				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		transformCamera.translate = CameraManager::GetInstance()->GetCamera()->GetTranslate();
		ImGui::DragFloat3("translate", &transformCamera.translate.x, 0.05f);
		CameraManager::GetInstance()->GetCamera()->SetTranslate(transformCamera.translate);
		transformCamera.rotate = CameraManager::GetInstance()->GetCamera()->GetRotate();
		ImGui::DragFloat3("rotate", &transformCamera.rotate.x, 0.05f);
		CameraManager::GetInstance()->GetCamera()->SetRotate(transformCamera.rotate);

		ImGui::Text("\n");
	}

	player_->DebugDraw();

	enemy_->DebugDraw();

	//int num = 0;
	// test
	/*for (auto& obj : testObjects_) {
		ImGui::PushID(obj.get() + num);
		if (ImGui::CollapsingHeader("testObj")) {
			obj->DebugDraw();
		}
		ImGui::PopID();
		num++;
	}*/

	// パーティクル
	ParticleManager::GetInstance()->Imgui();

	ImGui::End();

	//ImGui::SetNextWindowPos(ImVec2(900, 20), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	//ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	//ImGui::Begin("LIghting");

	//uint32_t objectCount = 0;
	//std::string objectName = "Object";
	//for (std::unique_ptr<Object3d>& object : objects_)
	//{
	//	objectName = objectName + std::to_string(objectCount);
	//	ImGui::PushID(objectName.c_str());
	//	if (ImGui::CollapsingHeader("Material"))
	//	{
	//		// 平行光源フラグ
	//		bool isEnableLighting = true;
	//		isEnableLighting = object->GetEnableLighting();

	//		if (isEnableLighting)
	//		{
	//			ImGui::PushID("DirectionalLight");
	//			if (ImGui::CollapsingHeader("DirectionalLight"))
	//			{
	//				// 平行光源
	//				MyBase::DirectionalLight directionalLight{};
	//				directionalLight = LightManager::GetInstance()->GetDirectionalLight();
	//				// 色
	//				ImGui::ColorEdit4("Color", &directionalLight.color.x);
	//				// 方向
	//				ImGui::SliderFloat3("Direction", &directionalLight.direction.x, -1, 1);
	//				// 輝度
	//				ImGui::DragFloat("Intensity", &directionalLight.intensity, 0.01f);
	//				LightManager::GetInstance()->SetDirectionalLight(directionalLight);
	//			}
	//			ImGui::PopID();
	//			ImGui::PushID("PointLight");
	//			if (ImGui::CollapsingHeader("PointLight"))
	//			{
	//				// 点光源
	//				MyBase::PointLight pointLight{};
	//				pointLight = LightManager::GetInstance()->GetPointLight();
	//				// 色
	//				ImGui::ColorEdit4("Color", &pointLight.color.x);
	//				// 位置
	//				ImGui::DragFloat3("Position", &pointLight.position.x, 0.01f);
	//				// 輝度
	//				ImGui::DragFloat("Intensity", &pointLight.intensity, 0.01f);
	//				// ライトの届く最大距離
	//				ImGui::DragFloat("Radius", &pointLight.radius, 0.01f, 0.0f);
	//				// 減衰率
	//				ImGui::DragFloat("Decay", &pointLight.decay, 0.01f, 0.0f);
	//				LightManager::GetInstance()->SetPointLight(pointLight);
	//			}
	//			ImGui::PopID();
	//			ImGui::PushID("SpotLight");
	//			if (ImGui::CollapsingHeader("SpotLight"))
	//			{
	//				// スポットライト
	//				MyBase::SpotLight spotLight{};
	//				spotLight = LightManager::GetInstance()->GetSpotLight();
	//				// 色
	//				ImGui::ColorEdit4("Color", &spotLight.color.x);
	//				// 位置
	//				ImGui::DragFloat3("Position", &spotLight.position.x, 0.01f);
	//				// 輝度
	//				ImGui::DragFloat("Intensity", &spotLight.intensity, 0.01f);
	//				// 方向
	//				ImGui::DragFloat3("Direction", &spotLight.direction.x, 0.01f);
	//				// ライトの届く最大距離
	//				ImGui::DragFloat("Distance", &spotLight.distance, 0.01f, 0.0f);
	//				// 減衰率
	//				ImGui::DragFloat("Decay", &spotLight.decay, 0.01f, 0.0f);
	//				// 余弦
	//				ImGui::SliderAngle("CosAngle", &spotLight.cosAngle);
	//				LightManager::GetInstance()->SetSpotLight(spotLight);
	//			}
	//			ImGui::PopID();
	//		}
	//	}
	//	objectCount++;
	//	ImGui::PopID();
	//}
	//ImGui::End();
}

// JSONファイルの読み込み
void GameScene::LoadJsonFile(const std::string& filePath)
{
	// レベルデータの読み込み
	LevelData* levelData = jsonLoader_->LoadFile(filePath);
	
	// 3Dオブジェクトの読み込み
	for (const ObjectData& objectData : levelData->objects) {
		// オブジェクトの種類ごとに処理
		if (objectData.name == "Player") {
			// プレイヤーの初期化
			player_->GetObject3d()->SetModel(objectData.objectName);
			player_->SetName(objectData.name.c_str());
			player_->GetObject3d()->SetTranslate(objectData.translation);
			player_->GetObject3d()->SetRotate(objectData.rotation);
			player_->GetObject3d()->SetScale(objectData.scale);
		}
		else if (objectData.name == "Enemy") {
			// 敵の初期化

		}
		else if (objectData.name == "Skydome") {
			// 天球の初期化

		}
		else {
			// その他のObjectはBaseObjectとして構築
			/*ModelManager::GetInstance()->LoadModel("debug/sphere", "sphere.obj");
			ModelManager::GetInstance()->LoadModel("debug/hummer", "hummer.obj");
			TextureManager::GetInstance()->LoadTexture("resources/texture/hummer.png");
			BaseObject* obj = CreateObjectFromData(objectData);
			testObjects_.emplace_back(obj);
			for (const auto& childData : objectData.children) {
				BaseObject* childObj = CreateObjectFromData(childData);
				childObj->GetObject3d()->GetWorldTransform()->SetParent(obj->GetObject3d()->GetWorldTransform());
				testObjects_.emplace_back(childObj);
			}*/
		}
	}
}
