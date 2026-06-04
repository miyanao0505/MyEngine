#include "PauseButton.h"
#include "MyTools.h"

using namespace std;

/// コンストラクタ
PauseButton::PauseButton(const PauseButtonDesc desc, const std::string& label)
	: type_(desc.type), label_(label)
{
	sprite_ = make_unique<Sprite>();
	sprite_->Initialize(desc.texturePath);
	sprite_->SetPosition(desc.position);
	sprite_->SetAnchorPoint(kAnchorPoint);
}

/// 更新
void PauseButton::Update(bool isSelected)
{
	// 選択判定によるサイズの更新
	MyBase::Vector2 size = isSelected ? MyTools::Multiply(kSelectMagnification, kButtonSize) : kButtonSize;
	ButtonSizeUpdate(size);

	// スプライトの更新
	sprite_->Update();
}

/// 描画
void PauseButton::Draw()
{
	sprite_->Draw();
}

/// ボタンのサイズ更新
void PauseButton::ButtonSizeUpdate(MyBase::Vector2 size)
{
	sprite_->SetSize(size);
}