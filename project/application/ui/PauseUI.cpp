#include "PauseUI.h"
#include "PauseInput.h"

using namespace std;

#pragma region 定数
const PauseButtonDesc PauseUI::kResumeButton = {
	PauseButtonType::Resume,
	"ResumeButton.png",
	{ 640.0f, 250.0f }
};
const PauseButtonDesc PauseUI::kGoToTitleButton = {
	PauseButtonType::GoToTitle,
	"GoToTitleButton.png",
	{ 640.0f, 470.0f }
};
#pragma endregion

/// 初期化
void PauseUI::Initialize()
{
	buttons_.emplace_back(make_unique<PauseButton>(kResumeButton, "戻る"));
	buttons_.emplace_back(make_unique<PauseButton>(kGoToTitleButton, "タイトルへ"));

	blackBackground_ = make_unique<Sprite>();
	blackBackground_->Initialize("white1x1.png");
	blackBackground_->SetSize(kScale);
	blackBackground_->SetPosition(kTranslate);
	blackBackground_->SetColor(kColor);
}

/// 更新
void PauseUI::Update(const PauseInput& input)
{
	if (input.IsSelectUp()) {
		MoveSelection(-1);
	}
	else if (input.IsSelectDown()) {
		MoveSelection(1);
	}

	if (input.IsDecide()) {
		decided_ = true;
	}

	for (size_t i = 0; i < buttons_.size(); ++i) {
		buttons_[i]->Update(i == selectedIndex_);
	}

	blackBackground_->Update();
}

/// 描画
void PauseUI::Draw()
{
	// 背景を黒くする(透明度高め)
	blackBackground_->Draw();

	for (size_t i = 0; i < buttons_.size(); ++i) {
		buttons_[i]->Draw();
	}
}

/// ポーズ開始時
void PauseUI::OnOpen()
{
	selectedIndex_ = 0;
	decided_ = false;
}

/// ポーズ終了時
void PauseUI::OnClose()
{
	decided_ = false;
}

PauseButtonType PauseUI::GetDecidedButton() const
{
	return buttons_[selectedIndex_]->GetType();
}

/// 選択移動
void PauseUI::MoveSelection(int dir)
{
	const int count = static_cast<int>(buttons_.size());
	selectedIndex_ = (selectedIndex_ + dir + count) % count;
}