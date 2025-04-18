#include "Model.h"
#include <fstream>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "ModelBase.h"
#include "Matrix.h"
#include "TextureManager.h"

// 初期化
void Model::Initialize(ModelBase* modelBase, const std::string& directorypath, const std::string& filename)
{
	// 引数で受け取ってメンバ変数に記録する
	modelBase_ = modelBase;

	// モデル読み込み
	LoadObjFile(directorypath, filename);

	// 頂点データの作成
	CreateVertexData();

	// マテリアルデータの作成
	CreateMaterialData();

	// .objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
	// 読み込んだテクスチャの番号を取得
	modelData_.material.textureIndex = TextureManager::GetInstance()->GetSrvIndex(modelData_.material.textureFilePath);

}

// 描画処理
void Model::Draw()
{
	// VBVの設定
	modelBase_->GetDxBase()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// マテリアルCBufferの場所を設定
	modelBase_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());
	// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	modelBase_->GetDxBase()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData_.material.textureFilePath));
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	modelBase_->GetDxBase()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}

// .mtlファイルの読み取り
MyBase::MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	/// 1. 中で必要となる変数の宣言
	MyBase::MaterialData materialData;	// 構築するMateriallData
	std::string line;					// ファイルが読んだ1行を格納するもの

	/// 2. ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);		// ファイルを開く
	assert(file.is_open());									// 取り敢えず開けなかったら止める

	/// 3. 実際にファイルを読み、MaterialDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textureFilePath = "resources/" + textureFilename;
		}
	}

	/// 4. MaterialDataを返す
	return materialData;
}

// .objファイルの読み取り
void Model::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	// 2. ファイルを開く
	Assimp::Importer importer;
	std::string filePath(directoryPath + "/" + filename);		// ファイルを開く
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());									// メッシュがないのは対応しない

	// 3. 実際にファイルを読み、ModelDataを構築していく
	// meshを解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());								// 法線がないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0));						// TexcoordがないMeshは今回は非対応
		// ここからMeshの中身(face)の解析を行っていく
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);						// 三角形のみサポート
			// ここからFaceの中身(Vertex)の解析を行っていく
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				MyBase::ModelVertexData vertex;
				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };
				// aiProcess_MakeLeftHandedは z*=-1 で、右手->左手に変換するので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData_.vertices.push_back(vertex);
			}
		}
	}
	// materialを解析
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			std::string directoryFilePath = "resources";
			modelData_.material.textureFilePath = directoryFilePath + "/" + textureFilePath.C_Str();
		}
	}
	// Scene全体の階層構造を作る
	modelData_.rootNode = ReadNode(scene->mRootNode);
}

void Model::SetTexture(const std::string& filename)
{
	modelData_.material.textureFilePath = filename;

	// .objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
	// 読み込んだテクスチャの番号を取得
	modelData_.material.textureIndex = TextureManager::GetInstance()->GetSrvIndex(modelData_.material.textureFilePath);
}

// 頂点データ作成
void Model::CreateVertexData()
{
	// 頂点リソースを作る
	vertexResource_ = modelBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::ModelVertexData) * modelData_.vertices.size());
	// 頂点バッファビューを作成する
	vertexBufferView_.BufferLocation = vertexResource_.Get()->GetGPUVirtualAddress();					// リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(MyBase::ModelVertexData) * modelData_.vertices.size());		// 使用するリソースのサイズは頂点のサイズ
	vertexBufferView_.StrideInBytes = sizeof(MyBase::ModelVertexData);										// 頂点あたりのサイズ

	// 頂点リソースにデータを書き込む
	vertexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));									// 書き込むためのアドレスを取得
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(MyBase::ModelVertexData) * modelData_.vertices.size());	// 頂点データをリソースにコピー

}

// マテリアルデータ作成
void Model::CreateMaterialData()
{
	// マテリアル用のリソースを作る
	materialResource_ = modelBase_->GetDxBase()->CreateBufferResource(sizeof(MyBase::ModelMaterial));
	// 書き込むためのアドレスを取得
	materialResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// 白で読み込む
	materialData_->color = MyBase::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	// Lightingを有効にする
	materialData_->enableLighting = true;
	// 単位行列で初期化
	materialData_->uvTransform = Matrix::MakeIdentity4x4();
	// 光沢度
	materialData_->shininess = 40.80f;
}

// Node情報を読み込む
MyBase::Node Model::ReadNode(aiNode* node)
{
	MyBase::Node result;

	aiMatrix4x4 aiLocalMatrix = node->mTransformation;	// nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose();							// 列ベクトル形式を行ベクトル形式に転置
	// 全てのの要素を移す
	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++) {
			result.localMatrix.m[i][j] = aiLocalMatrix[i][j];
		}
	}
	result.name = node->mName.C_Str();					// Node名を格納
	result.children.resize(node->mNumChildren);			// 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}
