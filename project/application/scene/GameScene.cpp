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
	TextureManager::GetInstance()->LoadTexture(filePath1_);
	TextureManager::GetInstance()->LoadTexture(filePath2_);
	//TextureManager::GetInstance()->LoadTexture(filePath3_);
	//TextureManager::GetInstance()->LoadTexture(filePath4_);

	// スプライト
	//for (uint32_t i = 0; i < 0; ++i)
	//{
	//	// スプライトの初期化
	//	std::unique_ptr<Sprite> sprite(new Sprite());
	//	sprite->Initialize(filePath1_);
	//	sprite->SetPosition({ 200.0f * float(i), 100.0f });
	//	sprite->SetSize({ 100.f, 100.f });
	//	sprite->SetAnchorPoint({ 0.0f, 0.0f });
	//	sprite->SetIsFlipX(false);
	//	sprite->SetIsFlipY(false);
	//	sprites_.push_back(std::move(sprite));
	//}

	//sprites_[1]->SetTexture(filePath2_);
	//sprites_[1]->SetSize({ 100.0f, 100.0f });
	//sprites_[3]->SetTexture(filePath2_);
	//sprites_[3]->SetSize({ 100.0f, 100.0f });
#pragma endregion スプライト

#pragma region 3Dオブジェクト
	// .objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel(modelFilePath1_.directoryPath, modelFilePath1_.filename);
	ModelManager::GetInstance()->LoadModel(modelFilePath2_.directoryPath, modelFilePath2_.filename);
	ModelManager::GetInstance()->LoadModel(modelFilePath3_.directoryPath, modelFilePath3_.filename);
	ModelManager::GetInstance()->LoadModel(modelFilePath4_.directoryPath, modelFilePath4_.filename);

	// 3Dオブジェクト
	//for (uint32_t i = 0; i < 0; ++i) {
	//	// 3Dオブジェクトの初期化
	//	std::unique_ptr<Object3d> object(new Object3d);
	//	object->Initislize();
	//	object->SetTranslate({ 0.0f, 0.0f, 0.0f });
	//	object->SetModel(modelFilePath4_.filename);
	//	// お試し用設定
	//	MyBase::DirectionalLight directionalLight{ .color{1.0f, 1.0f, 1.0f, 1.0f}, .direction{0.0f, 0.0f, 0.0f}, .intensity{0.0f} };
	//	LightManager::GetInstance()->SetDirectionalLight(directionalLight);
	//	MyBase::PointLight pointLight{ .color{1.0f, 1.0f, 1.0f, 1.0f}, .position{0.0f, 0.0f, 0.0f}, .intensity{0.0f}, .radius{5.0f}, .decay{1.0f} };
	//	LightManager::GetInstance()->SetPointLight(pointLight);
	//	MyBase::SpotLight spotLight{ .color{1.0f, 1.0f, 1.0f, 1.0f}, .position{2.0f, 1.25f, 0.0f}, .intensity{4.0f}, .direction{MyTools::Normalize({ -1.0f, -1.0f, 0.0f })}, .distance{7.0f}, .decay{1.0f}, .cosAngle{std::cosf(std::numbers::pi_v<float> / 3.0f)} };
	//	LightManager::GetInstance()->SetSpotLight(spotLight);
	//	objects_.push_back(std::move(object));
	//}
	//objects_[1]->SetModel(modelFilePath2_.filename);
	//objects_[2]->SetModel(modelFilePath3_.filename);

	// プレイヤー
	player_.reset(new Player);
	player_->Initialize({ 0.0f, 0.0f, 0.0f });
#pragma endregion 3Dオブジェクト

#pragma region パーティクル
	// パーティクル
	particleEmitter_.reset(new ParticleEmitter);
	particleEmitter_->Initialize("circle", "resources/circle.png");
#pragma endregion パーティクル

#pragma region オーディオ
	// BGM

	// お試し用
	AudioManager::GetInstance()->LoadAudioWave("fanfare.wav");
#pragma endregion オーディオ

#pragma region 変数
	isParticleActive_ = false;
	particleEmitter_->SetIsEmitUpdate(isParticleActive_);
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
	for (std::unique_ptr<Object3d>& object : objects_)
	{
		object.reset();
	}
	// スプライト
	for (std::unique_ptr<Sprite>& sprite : sprites_)
	{
		sprite.reset();
	}
}

// 毎フレーム更新
void GameScene::Update()
{
	BaseScene::Update();

#ifdef _DEBUG
	// Qキーを押したら
	if (input_->TriggerKey(DIK_Q)) {
		// お試しの音を解放する
		AudioManager::GetInstance()->UnLoadAudio("fanfare.wav");
	}
	// Rキーを押したら
	if (input_->TriggerKey(DIK_R)) {
		// お試しの音をロードする
		AudioManager::GetInstance()->LoadAudioWave("fanfare.wav");
	}

//	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Game");

	// デモウィンドウの表示オン
	//ImGui::ShowDemoWindow();
//
//	ImGui::Begin("Settings");
//

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
	if (ImGui::CollapsingHeader("3dObject"))
	{
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

		for (std::unique_ptr<Object3d>& object : objects_)
		{
			ImGui::PushID(object.get());
			if (ImGui::CollapsingHeader("Object"))
			{
				MyBase::Transform transform{ object->GetScale(), object->GetRotate(), object->GetTranslate() };

				// 移動
				ImGui::SliderFloat3("Translate", &transform.translate.x, -5.0f, 5.0f);
				object->SetTranslate(transform.translate);
				// 回転
				ImGui::SliderFloat3("Rotate", &transform.rotate.x, -3.14f, 3.14f);
				object->SetRotate(transform.rotate);
				// 拡縮
				ImGui::SliderFloat3("Scale", &transform.scale.x, 0.0f, 3.0f);
				object->SetScale(transform.scale);
			}
			ImGui::PopID();
		}

		ImGui::Text("\n");
	}
	//if (ImGui::CollapsingHeader("particle")) {
	//	static ImGuiComboFlags particleFlags = 0;
	//	const char* blendModeIndex[] = { "kBlendModeNone", "kBlendModeNormal", "kBlendModeAdd", "kBlendModeSubtract", "kBlendModeMultiply", "kBlendModeScreen" };
	//	static int selectID = 2;

	//	const char* previewValue = blendModeIndex[selectID];

	//	if (ImGui::BeginCombo("Now Blend", previewValue, particleFlags))
	//	{
	//		for (int n = 0; n < IM_ARRAYSIZE(blendModeIndex); n++)
	//		{
	//			const bool isSelected = (selectID == n);
	//			if (ImGui::Selectable(blendModeIndex[n], isSelected)) {
	//				selectID = n;
	//				ParticleManager::GetInstance()->ChangeBlendMode(static_cast<ParticleBase::BlendMode>(n));
	//			}
	//			if (isSelected) {
	//				ImGui::SetItemDefaultFocus();
	//			}
	//		}
	//		ImGui::EndCombo();
	//	}

	//	/*size_t spriteCount = 0;
	//	for (ParticleEmitter* particle : sprites) {*/
	//	MyBase::Vector3 position = particleEmitter_->GetPosition();
	//	ImGui::DragFloat2("particleEmitter_.Translate", &position.x, 0.1f);
	//	/*if (position.y > 640.0f) {
	//		position.y = 640.0f;
	//	}*/
	//	particleEmitter_->SetPosition(position);

	//	/*Vector3 rotation = particleEmitter_->GetRotation();
	//	ImGui::SliderAngle("particleEmitter_.Rotate", &rotation.x);
	//	particleEmitter_->SetRotation(rotation);

	//	Vector3 size = particleEmitter_->GetSize();
	//	ImGui::DragFloat2("particleEmitter_.Scale", &size.x, 0.1f);
	//	if (size.y > 360.0f) {
	//		size.y = 360.0f;
	//	}
	//	particleEmitter_->SetSize(size);*/

	//	int count = particleEmitter_->GetCount();
	//	ImGui::DragInt("particleEmitter_.count", &count, 1, 0, 1000);
	//	particleEmitter_->SetCount(count);

	//	float frequency = particleEmitter_->GetFrequency();
	//	ImGui::DragFloat("particleEmitter_.frequency", &frequency, 0.1f);
	//	particleEmitter_->SetFrequency(frequency);

	//	if (ImGui::Button("ParticleEmit", { 100,50 })) {
	//		particleEmitter_->Emit();
	//	}

	//	bool isEmitUpdate = particleEmitter_->GetIsEmitUpdate();
	//	ImGui::Checkbox("IsEmitUpdate", &isEmitUpdate);
	//	particleEmitter_->SetIsEmitUpdate(isEmitUpdate);

	//	ImGui::Checkbox("IsAccelerationField", &isAccelerationField_);
	//}

	//ImGui::Text("\n");

	//ImGui::Text("ParticleActive On / Off : SPACE");

//	}
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

	ImGui::SetNextWindowPos(ImVec2(900, 20), ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("LIghting");

	for (std::unique_ptr<Object3d>& object : objects_)
	{
		if (ImGui::CollapsingHeader("Material"))
		{
			// 平行光源フラグ
			bool isEnableLighting = true;
			isEnableLighting = object->GetEnableLighting();

			if (isEnableLighting)
			{
				ImGui::PushID("DirectionalLight");
				if (ImGui::CollapsingHeader("DirectionalLight"))
				{
					// 平行光源
					MyBase::DirectionalLight directionalLight{};
					directionalLight = LightManager::GetInstance()->GetDirectionalLight();
					// 色
					ImGui::ColorEdit4("Color", &directionalLight.color.x);
					// 方向
					ImGui::SliderFloat3("Direction", &directionalLight.direction.x, -1, 1);
					// 輝度
					ImGui::DragFloat("Intensity", &directionalLight.intensity, 0.01f);
					LightManager::GetInstance()->SetDirectionalLight(directionalLight);
				}
				ImGui::PopID();
				ImGui::PushID("PointLight");
				if (ImGui::CollapsingHeader("PointLight"))
				{
					// 点光源
					MyBase::PointLight pointLight{};
					pointLight = LightManager::GetInstance()->GetPointLight();
					// 色
					ImGui::ColorEdit4("Color", &pointLight.color.x);
					// 位置
					ImGui::DragFloat3("Position", &pointLight.position.x, 0.01f);
					// 輝度
					ImGui::DragFloat("Intensity", &pointLight.intensity, 0.01f);
					// ライトの届く最大距離
					ImGui::DragFloat("Radius", &pointLight.radius, 0.01f, 0.0f);
					// 減衰率
					ImGui::DragFloat("Decay", &pointLight.decay, 0.01f, 0.0f);
					LightManager::GetInstance()->SetPointLight(pointLight);
				}
				ImGui::PopID();
				ImGui::PushID("SpotLight");
				if (ImGui::CollapsingHeader("SpotLight"))
				{
					// スポットライト
					MyBase::SpotLight spotLight{};
					spotLight = LightManager::GetInstance()->GetSpotLight();
					// 色
					ImGui::ColorEdit4("Color", &spotLight.color.x);
					// 位置
					ImGui::DragFloat3("Position", &spotLight.position.x, 0.01f);
					// 輝度
					ImGui::DragFloat("Intensity", &spotLight.intensity, 0.01f);
					// 方向
					ImGui::DragFloat3("Direction", &spotLight.direction.x, 0.01f);
					// ライトの届く最大距離
					ImGui::DragFloat("Distance", &spotLight.distance, 0.01f, 0.0f);
					// 減衰率
					ImGui::DragFloat("Decay", &spotLight.decay, 0.01f, 0.0f);
					// 余弦
					ImGui::SliderAngle("CosAngle", &spotLight.cosAngle);
					LightManager::GetInstance()->SetSpotLight(spotLight);
				}
				ImGui::PopID();
			}
		}
	}
	ImGui::End();

#endif // _DEBUG

	// SPACEキーを押したら
	if (input_->TriggerKey(DIK_SPACE)) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	// Pキーを押したら
	if (input_->TriggerKey(DIK_P)) {
		// パーティクル描画フラグのOn / Off
		isParticleActive_ = particleEmitter_->GetIsEmitUpdate();
		isParticleActive_ = !isParticleActive_;
		particleEmitter_->SetIsEmitUpdate(isParticleActive_);
	}
	// Lキーを押したら
	if (input_->TriggerKey(DIK_L)) {
		// アクセラレーションのOn / Off
		isAccelerationField_ = !isAccelerationField_;
	}

	// Kキーを押したら
	if (input_->TriggerKey(DIK_K)) {
		// お試しの音を鳴らす
		AudioManager::GetInstance()->PlayWave("fanfare.wav");
	}
	// Oキーを押したら
	if (input_->TriggerKey(DIK_O)) {
		// お試しの音を解放する
		AudioManager::GetInstance()->UnLoadAudio("fanfare.wav");
	}
	// Lキーを押したら
	if (input_->TriggerKey(DIK_L)) {
		// お試しの音をロードする
		AudioManager::GetInstance()->LoadAudioWave("fanfare.wav");
	}

	// プレイヤーの更新処理
	player_->Update();

	// 3Dオブジェクトの更新処理
	for (std::unique_ptr<Object3d>& object : objects_)
	{
		object->Update();
	}

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
	particleEmitter_->Update();
	ParticleManager::GetInstance()->Update();

	// スプライトの更新処理
	for (std::unique_ptr<Sprite>& sprite : sprites_)
	{
		sprite->Update();
	}
}

// 描画
void GameScene::Draw()
{
#pragma region 3Dオブジェクト

	// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	ModelManager::GetInstance()->SetCommonScreen();

	// 全ての3DObject個々の描画
	for (std::unique_ptr<Object3d>& object : objects_)
	{
		object->Draw();
	}

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
	/*for (std::unique_ptr<Sprite>& sprite : sprites_)
	{
		sprite->Draw();
	}*/

#pragma endregion スプライト
}
