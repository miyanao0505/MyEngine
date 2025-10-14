#include "Characters.h"
#include "ModelManager.h"

// 初期化
void Characters::Initialize(const std::string& filePath, const std::string& fileName) {
	// .objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel(filePath, fileName);

	// 3Dオブジェクト
	titleCharFirst_ = std::make_unique<BaseObject>();
	titleCharFirst_->Initialize(fileName);
	titleCharFirst_->SetName("Enter");
	titleCharFirst_->GetObject3d()->SetScale({ 5.0f,5.0f,5.0f });
	titleCharFirst_->GetObject3d()->SetRotate({ 0.0f,0.0f,0.0f });
	titleCharFirst_->GetObject3d()->SetTranslate({ 0.0f,-2.50f,0.0f });
}

// 終了
void Characters::Finalize() {
	titleCharFirst_.reset();
}

// 更新
void Characters::Update() {
	titleCharFirst_->Update();
}

// 描画
void Characters::Draw() {
	titleCharFirst_->Draw();
}