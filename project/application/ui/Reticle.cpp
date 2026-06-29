#include "Reticle.h"
#include "Enemy.h"
#include "MyTools.h"
#include "WindowsAPI.h"

using namespace std;
using namespace MyBase;

/// 初期化
void Reticle::Initialize() {
	sprite_->Initialize("2DReticle.png");
	sprite_->SetPosition({ WindowsAPI::kClientWidth * 0.5f, WindowsAPI::kClientHeight * 0.5f });
}

/// 更新
void Reticle::Update() {
	sprite_->Update();
}

/// 描画
void Reticle::Draw() {
	if (sprite_) {
		sprite_->Draw();
	}
}