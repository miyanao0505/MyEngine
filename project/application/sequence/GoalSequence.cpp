#include "GoalSequence.h"
#include "Player.h"
#include "CameraManager.h"
#include "MyTools.h"
#include <algorithm>
#include <cmath>
#include <numbers>

using namespace std;
using namespace numbers;

namespace {
	float DegToRad(float deg) {
		return deg * pi_v<float> / 180.f;
	}
}

GoalSequence::GoalSequence()
{
}

/// 初期化
void GoalSequence::Initialize(Player* player, Camera* camera, const MyBase::Vector3& cameraOffset)
{
	player_ = player;
	cameraOffset_ = cameraOffset;
	cameraDistance_ = MyTools::Length(cameraOffset_);
	cameraHight_ = cameraOffset_.y;

	CameraManager::GetInstance()->AddCamera("GoalCamera");
	CameraManager::GetInstance()->SetCamera("GoalCamera");
	CameraManager::GetInstance()->GetCamera()->SetTransform(camera->GetTransform());

	clearLogo_ = std::make_unique<BaseObject>();
	clearLogo_->Initialize("characters", "clear.obj");
	clearLogo_->SetName("clearChar");
	clearLogo_->GetObject3D()->SetScale({ 0.0f, 0.0f, 0.0f });

	state_ = State::StartSet;
	stateTimer_ = 0.0f;
	isFinished_ = false;
}

/// 更新
void GoalSequence::Update(float deltaTime)
{
	switch (state_)
	{
	case GoalSequence::State::StartSet:
		UpdateStartSet();
		break;
	case GoalSequence::State::CameraRotate:
		UpdateCameraRotate(deltaTime);
		break;
	case GoalSequence::State::LogoScale:
		UpdateLogoScale(deltaTime);
		break;
	case GoalSequence::State::End:
		// 何もしない
		break;
	}
}

/// 描画
void GoalSequence::Draw()
{
	if (clearLogo_) {
		clearLogo_->Draw();
	}
}

#pragma region スマート更新
void GoalSequence::UpdateStartSet()
{
	stateTimer_ = 0.0f;
	currentAngle_ = 0.0f;

	state_ = State::CameraRotate;
}

void GoalSequence::UpdateCameraRotate(float deltaTime)
{
	stateTimer_ += deltaTime;

	float t = stateTimer_ / kRotateDuration;
	t = clamp(t, 0.0f, 1.0f);
	t = EaseInOut(t);

	currentAngle_ = 180.0f * t;
	UpdateCameraPosition(currentAngle_);

	if (t >= 1.0f) {
		stateTimer_ = 0.0f;
		state_ = State::LogoScale;
	}
}

void GoalSequence::UpdateLogoScale(float deltaTime)
{
	stateTimer_ += deltaTime;

	float t = stateTimer_ / kLogoScaleDuration;
	t = clamp(t, 0.0f, 1.0f);

	float scale = kLogoMaxScale * t;
	clearLogo_->GetObject3D()->SetScale({ scale, scale, scale });
	clearLogo_->Update();

	if (t >= 1.0f) {
		state_ = State::End;
		isFinished_ = true;
	}
}
#pragma endregion

#pragma region 補助処理
void GoalSequence::UpdateCameraPosition(float angleDeg)
{
	if (!player_ || !CameraManager::GetInstance()->GetCamera()) return;

	const MyBase::Vector3 playerPos = player_->GetWorldPosition();

	float rad = DegToRad(angleDeg);

	MyBase::Vector3 offset;
	offset.x = sin(rad) * cameraDistance_;
	offset.z = cos(rad) * cameraDistance_;
	offset.y = cameraHight_;

	MyBase::Vector3 cameraPos = MyTools::Add(playerPos, offset);

	CameraManager::GetInstance()->GetCamera()->SetTranslate(cameraPos);
}

float GoalSequence::EaseInOut(float t)
{
	// SmoothStep
	return t * t * (3.0f - 2.0f * t);
}
#pragma endregion