#include "StartSequence.h"
#include <DirectXMath.h>
#include "CameraManager.h"
#include "LightManager.h"
#include "MyBase.h"
#include "GameOverSequence.h"

using namespace DirectX;

StartSequence::StartSequence() {}

/// 初期化
void StartSequence::Initialize() {
	state_ = State::EngineStart;
	timer_ = 0.0f;
	isFinished_ = false;
	engineStarted_ = false;
	textSwitched_ = false;
	rotationY_ = 0.0f;
}

/// 更新
void StartSequence::Update(float deltaTime) {
	if (isFinished_) return;

	timer_ += deltaTime;

	switch (state_) {
	case State::EngineStart:
		EngineStartEffect();
		if (/*timer_ > 1.5f*/ timer_ > 0.5f) {
			TextSet();
			state_ = State::MissionText;
			timer_ = 0.0f;
		}
		break;
	case State::MissionText:
		if (timer_ > 1.0f) {
			state_ = State::MissionRotate;
			timer_ = 0.0f;
		}
		missionText_->Update();
		break;
	case State::MissionRotate:
		TextRotate(deltaTime);
		break;
	case State::StartText:
		if (timer_ > 1.0f) {
			HideText();
			isFinished_ = true;
			state_ = State::Finished;

			// ライトのリセット
			LightManager::GetInstance()->Initialize();
		}
		break;
	case State::Finished:
		// 何もしない
		break;
	}

}

/// 描画
void StartSequence::Draw() {
	if (missionText_) {
		missionText_->Draw();
		return;
	}
	if (startText_) {
		startText_->Draw();
	}
}

/// エンジン起動時のエフェクトを再生
void StartSequence::EngineStartEffect()
{
	if (!engineStarted_) {
		// ライトの設定
		MyBase::PointLight pointLight;
		pointLight.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		pointLight.position = CameraManager::GetInstance()->GetCamera()->GetTranslate();
		pointLight.intensity = 1.0f;
		pointLight.radius = 50.0f;
		pointLight.decay = 2.0f;
		LightManager::GetInstance()->SetPointLight(pointLight);

		// エンジン起動エフェクト再生処理
		/*const float amplitude = 5.0f;
		const float duration = 1.2f;
		const float frequency = 25.0f;
		const float rotationAmplitude = 0.03f;

		CameraManager::GetInstance()->StartShake(amplitude, duration, frequency, rotationAmplitude);*/

		engineStarted_ = true;
	}
}

/// テキストの設定
void StartSequence::TextSet()
{
	missionText_ = std::make_unique<BaseObject>();
	missionText_->Initialize("characters", "mission.obj");
	startText_ = std::make_unique<BaseObject>();
	startText_->Initialize("characters", "start.obj");

	MyBase::Transform transform = { {2.0f, 2.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 5.0f, -25.0f} };
	missionText_->GetObject3d()->SetTransform(transform);
	startText_->GetObject3d()->SetTransform(transform);

	// 初期更新
	missionText_->Update();
	startText_->Update();
}

/// テキストの回転状態を経過時間に応じて更新
void StartSequence::TextRotate(float deltaTime)
{
	rotationY_ += deltaTime * XMConvertToRadians(180.f);	// 1秒で180度回転

	if(!textSwitched_ && rotationY_ >= XMConvertToRadians(90.0f)) {
		// ミッションテキストを非表示にしてスタートテキストを表示
		missionText_.reset();
		textSwitched_ = true;
		
	}
	else if(missionText_)
	{
		missionText_->GetObject3d()->SetRotate({ 0.0f, rotationY_, 0.0f });
		missionText_->Update();
		return;
	}
	startText_->GetObject3d()->SetRotate({ 0.0f, rotationY_ + XMConvertToRadians(180.f), 0.0f });
	startText_->Update();

	if (rotationY_ >= XMConvertToRadians(180.0f)) {
		// 回転完了
		rotationY_ = 0.0f;
		state_ = State::StartText;
		timer_ = 0.0f;
	}
}

/// テキストを非表示
void StartSequence::HideText()
{
	startText_.reset();
}
