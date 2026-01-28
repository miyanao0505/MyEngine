#include "PauseController.h"
#include "PauseUI.h"
#include "PauseInput.h"
#include "SceneManager.h"

using namespace std;

PauseController::PauseController() = default;
PauseController::~PauseController() = default;

/// 初期化
void PauseController::Initialize()
{
	pauseUI_ = make_unique<PauseUI>();
	pauseInput_ = make_unique<PauseInput>();

	pauseUI_->Initialize();
}

/// 更新
void PauseController::Update()
{
	pauseInput_->Update();

	// ESCでトグル
	if (pauseInput_->IsPauseTriggered()) {
		isPaused_ ? ExitPause() : EnterPause();
	}

	if (!isPaused_) return;

	pauseUI_->Update(*pauseInput_);

	if (pauseUI_->HasDecided()) {
		switch (pauseUI_->GetDecidedButton())
		{
		case PauseButtonType::Resume:
			ExitPause();
			break;
		case PauseButtonType::GoToTitle:
			SceneManager::GetInstance()->ChangeScene(SceneName::Title);
			break;
		}
	}
}

/// 描画
void PauseController::Draw()
{
	if (isPaused_) {
		pauseUI_->Draw();
	}
}

/// ポーズ開始
void PauseController::EnterPause()
{
	isPaused_ = true;
	pauseUI_->OnOpen();
}

/// ポーズ終了
void PauseController::ExitPause()
{
	isPaused_ = false;
	pauseUI_->OnClose();
}