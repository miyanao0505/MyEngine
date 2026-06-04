#include "PauseInput.h"
#include "Input.h"

/// 更新
void PauseInput::Update()
{
	auto* input = Input::GetInstance();

	pauseTriggered_ = input->TriggerKey(DIK_ESCAPE);
	selectUp_ = input->TriggerKey(DIK_W);
	selectDown_ = input->TriggerKey(DIK_S);
	decide_ = input->TriggerKey(DIK_RETURN);
}