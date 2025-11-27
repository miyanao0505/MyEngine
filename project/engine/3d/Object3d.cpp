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
	// Object3dBase（描画パイプライン共通部）を取得して保持
	object3dBase_ = ModelManager::GetInstance()->GetObject3dBase();

	// モデルのセット
	SetModel(filePath);

	// 座標変換行列データの作成
	CreateTransformationMatrixData();

	// カメラデータの作成
	CreateCameraData();

	// ワールド変換クラスの作成と初期化
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
	
	// カメラ情報の更新
	// 指定したカメラが存在する場合は WVP を作成
	if (CameraManager::GetInstance()->GetCamera()) {
		// WVP 行列の計算
		const MyBase::Matrix4x4& viewProjectionMatrix = CameraManager::GetInstance()->GetCamera()->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Matrix::Multiply(worldTransform_->GetWorldMatrix(), viewProjectionMatrix);
		
		// カメラのワールド座標をセット
		cameraData_->worldPosition = CameraManager::GetInstance()->GetCamera()->GetTranslate();
	}
	// 指定したカメラが存在しない場合はワールド行列のみ
	else {
		worldViewProjectionMatrix = worldTransform_->GetWorldMatrix();
	}

	// 座標変換行列データの更新
	transformationMatrixData_->WVP = Matrix::Multiply(model_->GetModelData().rootNode.localMatrix, worldViewProjectionMatrix);
	transformationMatrixData_->World = Matrix::Multiply(model_->GetModelData().rootNode.localMatrix, worldTransform_->GetWorldMatrix());
	transformationMatrixData_->WorldInverseTranspose = Matrix::Transpose(Matrix::Inverse(worldTransform_->GetWorldMatrix()));
}

// 描画処理
void Object3d::Draw()
{
	// ルートパラメータ 1：座標変換行列(WVP)用のCBufferの場所を設定
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_.Get()->GetGPUVirtualAddress());
	// ルートパラメータ 4：カメラ情報用のCBufferの場所を設定
	object3dBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResource_.Get()->GetGPUVirtualAddress());
	// LightManager 内部で必要な CBuffer をセット
	LightManager::GetInstance()->Draw(object3dBase_);

	// モデルが割り当てられている場合のみ描画
	if (model_) {
		// 描画直前に使用するテクスチャ名を指定
		model_->SetTexture(textureFileName_);
		model_->Draw();
	}
}

// モデルファイルを指定して読み込み・設定
void Object3d::SetModel(const std::string& filePath)
{
	// 既に読み込まれているモデルを検索
	model_ = ModelManager::GetInstance()->FindModel(filePath);

	// 未ロードなら読み込む
	if (!model_) {
		// ファイルパスからフォルダパスを抽出
		size_t dotPos = filePath.find('.');
		const std::string folderPath = (dotPos != std::string::npos) ? filePath.substr(0, dotPos) : filePath;
		
		// モデル読み込み・設定
		ModelManager::GetInstance()->LoadModel(folderPath, filePath);
		model_ = ModelManager::GetInstance()->FindModel(filePath);
	}
}

// 描画に使用するテクスチャを変更
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
