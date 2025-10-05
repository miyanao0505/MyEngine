#include "TitleLogo.h"
#include "ModelManager.h"

// 初期化
void TitleLogo::Initialize() {
	// .objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel("charactors", "title_1.obj");
	ModelManager::GetInstance()->LoadModel("charactors", "title_2.obj");
	ModelManager::GetInstance()->LoadModel("charactors", "title_3.obj");

	// 3Dオブジェクト
	titleCharFirst_ = std::make_unique<BaseObject>();
	titleCharFirst_->Initialize("title_1.obj");
	titleCharFirst_->SetName("TitleObject1");
	titleCharFirst_->GetObject3d()->SetScale({ 5.0f,5.0f,5.0f });
	titleCharFirst_->GetObject3d()->SetRotate({ 0.0f,0.0f,0.0f });
	titleCharFirst_->GetObject3d()->SetTranslate({ -5.0f,5.0f,0.0f });
	titleCharSecond_ = std::make_unique<BaseObject>();
	titleCharSecond_->Initialize("title_2.obj");
	titleCharSecond_->SetName("TitleObject2");
	titleCharSecond_->GetObject3d()->SetScale({ 5.0f,5.0f,5.0f });
	titleCharSecond_->GetObject3d()->SetRotate({ 0.0f,0.0f,0.0f });
	titleCharSecond_->GetObject3d()->SetTranslate({ 0.0f,5.0f,0.0f });
	titleCharThird_ = std::make_unique<BaseObject>();
	titleCharThird_->Initialize("title_3.obj");
	titleCharThird_->SetName("TitleObject3");
	titleCharThird_->GetObject3d()->SetScale({ 5.0f,5.0f,5.0f });
	titleCharThird_->GetObject3d()->SetRotate({ 0.0f,0.0f,0.0f });
	titleCharThird_->GetObject3d()->SetTranslate({ 5.0f,5.0f,0.0f });
}

// 終了
void TitleLogo::Finalize() {
	titleCharThird_.reset();
	titleCharSecond_.reset();
	titleCharFirst_.reset();
}

// 更新
void TitleLogo::Update() {
	if (titleCharFirst_) {
		titleCharFirst_->Update();
	}
	if (titleCharSecond_) {
		titleCharSecond_->Update();
	}
	if (titleCharThird_) {
		titleCharThird_->Update();
	}
}

// 描画
void TitleLogo::Draw() {
	if (titleCharFirst_) {
		titleCharFirst_->Draw();
	}
	if (titleCharSecond_) {
		titleCharSecond_->Draw();
	}
	if (titleCharThird_) {
		titleCharThird_->Draw();
	}
}