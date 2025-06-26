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
#include <numbers>


// 初期化
void GameScene::Initialize()
{
	BaseScene::Initialize();

#pragma region スプライト
	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(gameTextureFilePath_);

	// スプライト
	gameSprite_.reset(new Sprite);
	gameSprite_->Initialize(gameTextureFilePath_);
	gameSprite_->SetPosition({ 0.0f, 0.0f });	// スプライトの位置を設定
#pragma endregion スプライト

#pragma region 3Dオブジェクト
	// .objファイルからモデルを読み込む
	

	// プレイヤー
	player_.reset(new Player);
	player_->Initialize({ 0.0f, 0.0f, 0.0f });

	// 敵
	enemy_.reset(new Enemy);
	enemy_->Initialize();

	// 天球
	skydome_.reset(new Skydome);
	skydome_->Initialize({ 0.0f, 0.0f, 0.0f });
#pragma endregion 3Dオブジェクト

#pragma region パーティクル
	// パーティクル
	
#pragma endregion パーティクル

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
}

// 終了
void GameScene::Finalize()
{
	BaseScene::Finalize();

	// 3Dオブジェクト
	
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

	// スプライト
//	if (ImGui::CollapsingHeader("Sprite"))
//	{
//		// ブレンドモード
//		if (ImGui::CollapsingHeader("BlendModeSprite")) {
//			static ImGuiComboFlags spriteFlags = 0;
//			const char* blendModeIndex[] = { "kBlendModeNone", "kBlendModeNormal", "kBlendModeAdd", "kBlendModeSubtract", "kBlendModeMultiply", "kBlendModeScreen" };
//			static int selectID = 1;
//
//			const char* previewValue = blendModeIndex[selectID];
//
//			if (ImGui::BeginCombo("now Blend", previewValue, spriteFlags))
//			{
//				for (int n = 0; n < IM_ARRAYSIZE(blendModeIndex); n++)
//				{
//					const bool isSelected = (selectID == n);
//					if (ImGui::Selectable(blendModeIndex[n], isSelected)) {
//						selectID = n;
//						spriteBase_->SetBlendMode(static_cast<SpriteBase::BlendMode>(n));
//					}
//
//					if (isSelected) {
//						ImGui::SetItemDefaultFocus();
//					}
//				}
//				ImGui::EndCombo();
//			}
//		}
//		for (std::unique_ptr<Sprite>& sprite : sprites_)
//		{
//			ImGui::PushID(sprite.get());
//			if (ImGui::CollapsingHeader("Object"))
//			{
//				// 移動
//				MyBase::Vector2 translate = sprite->GetPosition();
//				ImGui::SliderFloat2("Translate", &translate.x, 0.0f, 640.0f);
//				sprite->SetPosition(translate);
//				// 回転
//				float rotation = sprite->GetRotation();
//				ImGui::SliderAngle("Rotate", &rotation);
//				sprite->SetRotation(rotation);
//				// 拡縮
//				MyBase::Vector2 size = sprite->GetSize();
//				ImGui::SliderFloat2("Scale", &size.x, 0.0f, 640.f);
//				sprite->SetSize(size);
//				// アンカーポイント
//				MyBase::Vector2 anchorPoint = sprite->GetAnchorPoint();
//				ImGui::DragFloat2("AnchorPoint", &anchorPoint.x, 0.05f, -1.0f, 2.0f);
//				sprite->SetAnchorPoint(anchorPoint);
//				// フリップ
//				bool isFlipX = sprite->GetIsFlipX();
//				ImGui::Checkbox("isFlipX", &isFlipX);
//				sprite->SetIsFlipX(isFlipX);
//				bool isFlipY = sprite->GetIsFlipY();
//				ImGui::Checkbox("isFlipY", &isFlipY);
//				sprite->SetIsFlipY(isFlipY);
//				// テクスチャ範囲指定
//				MyBase::Vector2 textureLeftTop = sprite->GetTextureLeftTop();
//				ImGui::SliderFloat2("textureLeftTop", &textureLeftTop.x, 0.0f, max(sprite->GetSpriteSize().x, sprite->GetSpriteSize().y));
//				sprite->SetTextureLeftTop(textureLeftTop);
//				MyBase::Vector2 textureSize = sprite->GetTextureSize();
//				ImGui::SliderFloat2("textureSize", &textureSize.x, 0.0f, max(sprite->GetSpriteSize().x, sprite->GetSpriteSize().y) * 2.f);
//				sprite->SetTextureSize(textureSize);
//
//				if (ImGui::CollapsingHeader("Material"))
//				{
//					// 色
//					MyBase::Vector4 color = sprite->GetColor();
//					ImGui::ColorEdit4("color", &color.x);
//					sprite->SetColor(color);
//				}
//			}
//			ImGui::PopID();
//		}
//	}
//
	// 3Dオブジェクト
	/*MyBase::Vector3 rotate = objects_[0]->GetRotate();
	rotate.y += 0.02f;
	objects_[0]->SetRotate(rotate);
	rotate = objects_[1]->GetRotate();
	rotate.z += 0.02f;
	objects_[1]->SetRotate(rotate);*/
	//if (ImGui::CollapsingHeader("3dObject"))
	//{
		// ブレンドモード
		/*if (ImGui::CollapsingHeader("BlendMode3dObject")) {
			static ImGuiComboFlags spriteFlags = 0;
			const char* blendModeIndex[] = { "kBlendModeNone", "kBlendModeNormal", "kBlendModeAdd", "kBlendModeSubtract", "kBlendModeMultiply", "kBlendModeScreen" };
			static int selectID = 1;

			const char* previewValue = blendModeIndex[selectID];

			if (ImGui::BeginCombo("now Blend", previewValue, spriteFlags))
			{
				for (int n = 0; n < IM_ARRAYSIZE(blendModeIndex); n++)
				{
					const bool isSelected = (selectID == n);
					if (ImGui::Selectable(blendModeIndex[n], isSelected)) {
						selectID = n;
						ModelManager::GetInstance()->GetObject3dBase()->SetBlendMode(static_cast<Object3dBase::BlendMode>(n));
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}*/

		//for (std::unique_ptr<Object3d>& object : objects_)
		//{
		//	ImGui::PushID(object.get());
		//	if (ImGui::CollapsingHeader("Object"))
		//	{
		//		MyBase::Transform transform{ object->GetScale(), object->GetRotate(), object->GetTranslate() };

		//		// 移動
		//		ImGui::SliderFloat3("Translate", &transform.translate.x, -5.0f, 5.0f);
		//		object->SetTranslate(transform.translate);
		//		// 回転
		//		ImGui::SliderFloat3("Rotate", &transform.rotate.x, -3.14f, 3.14f);
		//		object->SetRotate(transform.rotate);
		//		// 拡縮
		//		ImGui::SliderFloat3("Scale", &transform.scale.x, 0.0f, 3.0f);
		//		object->SetScale(transform.scale);
		//	}
		//	ImGui::PopID();
		//}

		//ImGui::Text("\n");
	//}
	
	// パーティクル
	ParticleManager::GetInstance()->Imgui();
	

	//ImGui::Text("\n");

	//}
//
//	//// テクスチャ
//	////ImGui::Checkbox("useMonsterBall", &useMonsterBall);
//
//	//// UV
//	///*ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
//	//ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
//	//ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);*/
//
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

#endif // _DEBUG

	// 3Dオブジェクトの更新処理
	// プレイヤーの更新処理
	player_->Update();

	// 敵の更新処理
	enemy_->Update();

	// 天球の更新
	skydome_->Update();

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
