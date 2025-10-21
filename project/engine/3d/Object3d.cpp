#include "Object3d.h"
#include <fstream>
#include "Object3dBase.h"
#include "ModelManager.h"
#include "Matrix.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "LightManager.h"

// 初期化
void Object3d::Initislize(const std::string& filePath)
{
	// メンバ変数に記録する
	object3dBase_ = ModelManager::GetInstance()->GetObject3dBase();
	SetModel(filePath);

	// 座標変換行列データの作成
	CreateTransformationMatrixData();

	// カメラデータの作成
	CreateCameraData();

	// 3DオブジェクトのTransformの初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// Texture
	textureFileName_ = model_->GetTexture();
}

// 更新処理
void Object3d::Update()
{
	// WorldMatrixの作成
	worldTransform_->UpdateWorldMatrix();
	MyBase::Matrix4x4 worldViewProjectionMatrix;
	if (CameraManager::GetInstance()->GetCamera()) {
		const MyBase::Matrix4x4& viewProjectionMatrix = CameraManager::GetInstance()->GetCamera()->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Matrix::Multiply(worldTransform_->GetWorldMatrix(), viewProjectionMatrix);
		cameraData_->worldPosition = CameraManager::GetInstance()->GetCamera()->GetTranslate();
	} else {
		worldViewProjectionMatrix = worldTransform_->GetWorldMatrix();
	}
	transformationMatrixData_->WVP = Matrix::Multiply(model_->GetModelData().rootNode.localMatrix, worldViewProjectionMatrix);
	transformationMatrixData_->World = Matrix::Multiply(model_->GetModelData().rootNode.localMatrix, worldTransform_->GetWorldMatrix());
	transformationMatrixData_->WorldInverseTranspose = Matrix::Transpose(Matrix::Inverse(worldTransform_->GetWorldMatrix()));
}

// 描画処理
void Object3d::Draw()
{
	// WVP用のCBufferの場所を設定
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_.Get()->GetGPUVirtualAddress());
	// カメラ用のCBufferの場所を設定
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResource_.Get()->GetGPUVirtualAddress());
	// ライトの設定
	LightManager::GetInstance()->Draw(object3dBase_);

	// 3Dモデルが割り当てられていれば描画する
	if (model_) {
		model_->SetTexture(textureFileName_);
		model_->Draw();
	}
}

void Object3d::SetModel(const std::string& filePath)
{
	// モデルを検索してセットする
	model_ = ModelManager::GetInstance()->FindModel(filePath);

	if (!model_) {
		size_t dotPos = filePath.find('.');
		const std::string folderPath = (dotPos != std::string::npos) ? filePath.substr(0, dotPos) : filePath;
		ModelManager::GetInstance()->LoadModel(folderPath, filePath);
		model_ = ModelManager::GetInstance()->FindModel(filePath);
	}
}

void Object3d::SetTexture(const std::string& filename)
{
	textureFileName_ = filename;
}

// 座標変換行列データ作成
void Object3d::CreateTransformationMatrixData()
{
	// TransformationMatrix用のリソースを作る
	transformationMatrixResource_ = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::TransformationMatrix));
	// 書き込むためのアドレスを取得
	transformationMatrixResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	// 単位行列を書き込んでおく
	transformationMatrixData_->WVP = Matrix::MakeIdentity4x4();
	transformationMatrixData_->World = Matrix::MakeIdentity4x4();
	transformationMatrixData_->WorldInverseTranspose = Matrix::MakeIdentity4x4();
}

// カメラデータ作成
void Object3d::CreateCameraData()
{
	// カメラ用のリソースを作る
	cameraResource_ = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::CameraForGPU));
	// 書き込むためのアドレス取得
	cameraResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	cameraData_->worldPosition = { 0.0f, 0.0f, 0.0f };
	if (CameraManager::GetInstance()->GetCamera()) {
		cameraData_->worldPosition = CameraManager::GetInstance()->GetCamera()->GetTranslate();
	}
}
