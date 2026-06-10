#include "Reticle.h"

using namespace std;
using namespace MyBase;

/// 初期化
void Reticle::Initialize() {
	// レティクル2D
	reticle2D_ = make_unique<Sprite>();
	reticle2D_->Initialize("2DReticle.png");
	
	// レティクル3D
	reticle3D_ = make_unique<BaseObject>();
	reticle3D_->Initialize("debug/cube", "cube.obj");
}

/// 更新
void Reticle::Update() {
	// レティクル2D
	reticle2D_->Update();

	// レティクル3D
	reticle3D_->Update();
}

/// 描画
void Reticle::Draw() {

}