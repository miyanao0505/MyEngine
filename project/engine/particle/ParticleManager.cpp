#include "ParticleManager.h"
#include "DirectXBase.h"
#include"SrvManager.h"
#include"TextureManager.h"
#include"CameraManager.h"
#include "MyTools.h"
#include "Matrix.h"
#include<numbers>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


ParticleManager* ParticleManager::instance = nullptr;

ParticleManager* ParticleManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ParticleManager;
	}
	return instance;
}

void ParticleManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void ParticleManager::Initialize(DirectXBase* dxBase, SrvManager* srvManager)
{
	dxBase_ = dxBase;
	srvManager_ = srvManager;

	particleBase_ = std::make_unique_for_overwrite<ParticleBase>();
	particleBase_->Initislize(dxBase_);

	// ランダムエンジンの初期化
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
}

void ParticleManager::Update()
{
	MyBase::Matrix4x4 viewProjectionMatrix;
	MyBase::Matrix4x4 billoardMatrix;

	if (CameraManager::GetInstance()->GetCamera()) {
		viewProjectionMatrix = CameraManager::GetInstance()->GetCamera()->GetViewProjectionMatrix();
		billoardMatrix = CameraManager::GetInstance()->GetCamera()->GetWorldMatrix();
		billoardMatrix.m[3][0] = 0.0f;
		billoardMatrix.m[3][1] = 0.0f;
		billoardMatrix.m[3][2] = 0.0f;
	}
	for (std::pair<const std::string, std::unique_ptr<ParticleGroup>>& pair : particleGroups_) {
		ParticleGroup& group = *pair.second;
		int index = 0;
		for (std::list<MyBase::Particle>::iterator it = group.particles.begin(); it != group.particles.end();) {
			MyBase::Particle& particle = *it;

			particle.currentTime += kDelTime_;
			particle.color.w = 1.0f - (particle.currentTime / particle.lifeTime);
			particle.transform.translate = MyTools::Add(particle.transform.translate, MyTools::Multiply(kDelTime_, particle.velocity));
			if (particle.lifeTime <= particle.currentTime) {
				it = group.particles.erase(it);
				group.kNumInstance--;
				continue;
			}
			billoardMatrix = Matrix::Multiply(billoardMatrix, Matrix::MakeRotateZMatrix4x4(particle.transform.rotate.z));
			MyBase::Matrix4x4 worldMatrix = Matrix::Multiply(Matrix::Multiply(Matrix::MakeScaleMatrix(particle.transform.scale), billoardMatrix), Matrix::MakeTranslateMatrix(particle.transform.translate));
			MyBase::Matrix4x4 worldViewProjectionMatrix = Matrix::Multiply(worldMatrix, viewProjectionMatrix);
			group.instancingData[index].World = worldMatrix;
			group.instancingData[index].WVP = worldViewProjectionMatrix;
			group.instancingData[index].color = particle.color;

			++it;
			++index;
		}
	}
}

void ParticleManager::Draw()
{
	// ルートシグネチャを設定
	particleBase_->SetCommonScreen();

	// 全てのパーティクルグループについて処理
	for (auto& [name, group] : particleGroups_) {
		dxBase_->GetCommandList()->IASetVertexBuffers(0, 1, &group->vertexBufferView);	// VBVを設定
		dxBase_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);					// IBAを設定
		// インスタンシングデータのSRVのDescriptorTableを設定
		srvManager_->SetGraphicsRootDescriptorTable(0, group->srvIndexForInstancing);
		// SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
		dxBase_->GetCommandList()->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetSrvHandleGPU(group->materialData.textureFilePath));
		// DrawCall(インスタンシング描画)
		dxBase_->GetCommandList()->DrawIndexedInstanced(kParticleIndexNum[group->type], group->kNumInstance, 0, 0, 0);
	}
}

#ifdef _DEBUG
void ParticleManager::Imgui()
{
	if (ImGui::CollapsingHeader("ParticleManager")) {
		static ImGuiComboFlags particleFlags = 0;
		const char* blendModeIndex[] = { "kBlendModeNone", "kBlendModeNormal", "kBlendModeAdd", "kBlendModeSubtract", "kBlendModeMultiply", "kBlendModeScreen" };
		static int selectID = 2;

		const char* previewValue = blendModeIndex[selectID];

		if (ImGui::BeginCombo("Now Blend", previewValue, particleFlags)) {
			for (int n = 0; n < IM_ARRAYSIZE(blendModeIndex); n++)
			{
				const bool isSelected = (selectID == n);
				if (ImGui::Selectable(blendModeIndex[n], isSelected)) {
					selectID = n;
					ChangeBlendMode(static_cast<ParticleBase::BlendMode>(n));
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::Text("ParticleGroupNum : %d", particleGroups_.size());

	}
}
#endif // _DEBUG

void ParticleManager::ChangeBlendMode(ParticleBase::BlendMode blendMode)
{
	if (particleBase_->GetBlendMode() == blendMode) {
		return;
	}

	particleBase_->SetBlendMode(blendMode);
	particleBase_->CreateGraphicsPipeline();
}

void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath)
{
	if (particleGroups_.count(name) != 0) {
		return;
	}

	assert(particleGroups_.count(name) == 0 && "ParticleGroup with this name already exists");

	// パーティクルグループの作成と初期化
	auto group = std::make_unique<ParticleGroup>();
	// .objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(textureFilePath);
	group->materialData.textureFilePath = textureFilePath;

	group->type = ParticleEmitter::Box;

	group->kNumInstance = 0;

	// 頂点リソースの生成
	group->vertexResource = dxBase_->CreateBufferResource(sizeof(MyBase::ParticleVertexData) * kParticleVertexNum);
	
	// 頂点バッファビューの生成
	group->vertexBufferView.BufferLocation = group->vertexResource->GetGPUVirtualAddress();
	group->vertexBufferView.SizeInBytes = sizeof(MyBase::ParticleVertexData) * kParticleVertexNum;
	group->vertexBufferView.StrideInBytes = sizeof(MyBase::ParticleVertexData);
	// 頂点リソースに頂点データを書き込む
	group->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&group->vertexData));
	// テクスチャの頂点
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(textureFilePath);
	textureSize_.x = static_cast<float>(metadata.width);
	textureSize_.y = static_cast<float>(metadata.height);

	float texLeft = textureLeftTop_.x / metadata.width;
	float texRight = (textureLeftTop_.x + textureSize_.x) / metadata.width;
	float texTop = textureLeftTop_.y / metadata.height;
	float texBottom = (textureLeftTop_.y + textureSize_.y) / metadata.height;

	// 頂点データを設定(四角形を構成)
	group->vertexData[0] = { { -0.5f, -0.5f, 0.0f, 1.0f }, { texLeft, texBottom  }, { 1.0f, 1.0f, 1.0f, 1.0f } };
	group->vertexData[1] = { { -0.5f,  0.5f, 0.0f, 1.0f }, { texLeft, texTop     }, { 1.0f, 1.0f, 1.0f, 1.0f } };
	group->vertexData[2] = { {  0.5f, -0.5f, 0.0f, 1.0f }, { texRight, texBottom }, { 1.0f, 1.0f, 1.0f, 1.0f } };
	group->vertexData[3] = { {  0.5f,  0.5f, 0.0f, 1.0f }, { texRight, texTop }, { 1.0f, 1.0f, 1.0f, 1.0f } };

	// TextureManagerからGPUハンドルを取得
	group->materialData.textureIndex = TextureManager::GetInstance()->GetSrvIndex(textureFilePath);

	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	group->instancingResource = dxBase_->CreateBufferResource(sizeof(MyBase::ParticleForGPU) * kMaxInstance_);
	// 書き込むためのアドレスを取得
	group->instancingResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&group->instancingData));
	// 単位行列を書き込んでおく
	for (uint32_t index = 0; index < kMaxInstance_; index++) {
		group->instancingData[index].World = Matrix::MakeIdentity4x4();
		group->instancingData[index].WVP = Matrix::MakeIdentity4x4();
		group->instancingData[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	group->srvIndexForInstancing = srvManager_->Allocate();
	srvManager_->CreateSRVforStructuredBuffer(group->srvIndexForInstancing, group->instancingResource.Get(), kMaxInstance_, sizeof(MyBase::ParticleForGPU));

	particleGroups_[name] = std::move(group);
}

/// パーティクルグループ(Ring)の生成
void ParticleManager::CreateParticleGroupRing(const std::string name, const std::string textureFilePath)
{
	if (particleGroups_.count(name) != 0) {
		return;
	}

	assert(particleGroups_.count(name) == 0 && "ParticleGroup with this name already exists");

	// パーティクルグループの作成と初期化
	auto group = std::make_unique<ParticleGroup>();
	// .objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(textureFilePath);
	group->materialData.textureFilePath = textureFilePath;

	group->type = ParticleEmitter::Ring;

	group->kNumInstance = 0;

	// 頂点リソースの生成
	group->vertexResource = dxBase_->CreateBufferResource(size_t(sizeof(MyBase::ParticleVertexData) * kParticleVertexNum * kRingDivide));

	// 頂点バッファビューの生成
	group->vertexBufferView.BufferLocation = group->vertexResource->GetGPUVirtualAddress();
	group->vertexBufferView.SizeInBytes = size_t(sizeof(MyBase::ParticleVertexData) * kParticleVertexNum * kRingDivide);
	group->vertexBufferView.StrideInBytes = sizeof(MyBase::ParticleVertexData);
	// 頂点リソースに頂点データを書き込む
	group->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&group->vertexData));
	// テクスチャの頂点
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(textureFilePath);
	textureSize_.x = static_cast<float>(metadata.width);
	textureSize_.y = static_cast<float>(metadata.height);

	// 頂点データを設定
	for (uint32_t index = 0; index < kRingDivide; index++) {
		float sin = std::sinf(index * kRadianPerDivide);
		float cos = std::cosf(index * kRadianPerDivide);
		float sinNext = std::sinf((index + 1) * kRadianPerDivide);
		float cosNext = std::cosf((index + 1) * kRadianPerDivide);
		float u = float(index) / float(kRingDivide);
		float uNext = float(index + 1) / float(kRingDivide);

		// positionとuv。normalは必要なら +z を設定する
		group->vertexData[index * 4] = { { -sin * kOuterRadius, cos * kOuterRadius, 0.0f, 1.0f }, { u, 0.0f  }, { 1.0f, 1.0f, 1.0f, 1.0f } };
		group->vertexData[index * 4 + 1] = { { -sinNext * kOuterRadius, cosNext * kOuterRadius, 0.0f, 1.0f }, { uNext, 0.0f     }, { 1.0f, 1.0f, 1.0f, 1.0f } };
		group->vertexData[index * 4 + 2] = { { -sin * kInnerRadius, cos * kInnerRadius, 0.0f, 1.0f }, { u, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } };
		group->vertexData[index * 4 + 3] = { { -sinNext * kInnerRadius, cosNext * kInnerRadius, 0.0f, 1.0f }, { uNext, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } };
	}

	// TextureManagerからGPUハンドルを取得
	group->materialData.textureIndex = TextureManager::GetInstance()->GetSrvIndex(textureFilePath);

	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	group->instancingResource = dxBase_->CreateBufferResource(size_t(sizeof(MyBase::ParticleForGPU) * kMaxInstance_));
	// 書き込むためのアドレスを取得
	group->instancingResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&group->instancingData));
	
	// 単位行列を書き込んでおく
	for (uint32_t index = 0; index < kMaxInstance_; index++) {
		group->instancingData[index].World = Matrix::MakeIdentity4x4();
		group->instancingData[index].WVP = Matrix::MakeIdentity4x4();
		group->instancingData[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	group->srvIndexForInstancing = srvManager_->Allocate();
	srvManager_->CreateSRVforStructuredBuffer(group->srvIndexForInstancing, group->instancingResource.Get(), UINT(kMaxInstance_), sizeof(MyBase::ParticleForGPU));

	particleGroups_[name] = std::move(group);
}

void ParticleManager::Emit(const std::string name, const MyBase::Vector3& position, uint32_t count)
{
	assert(particleGroups_.count(name) > 0 && "ParticleGroup with this name does not exist.");

	ParticleGroup& group = *particleGroups_[name];
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	uint32_t nowInstance = group.kNumInstance;
	group.kNumInstance += count;
	if (group.kNumInstance + count >= kMaxInstance_) {
		group.kNumInstance = kMaxInstance_;
	}
	for (uint32_t i = nowInstance; i < group.kNumInstance; ++i) {
		//group.particles.push_back(CreateParticle(randomEngine, position));
		group.particles.push_back(CreateHitParticle(randomEngine, position, group.type));
	}
}

void ParticleManager::CreateIndexResource(ParticleEmitter::ParticleType type)
{
	/*uint32_t preIndexNum = particleIndexSize_;*/

	particleIndexSize_ += kParticleIndexNum[type];

	// インデックスリソースの生成
	indexResource_ = dxBase_->CreateBufferResource(sizeof(uint32_t) * kParticleIndexNum[type]);

	// インデックスバッファビューの生成
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * kParticleIndexNum[type];
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	if (type == ParticleEmitter::Box) {
		indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
		indexData_[3] = 1; indexData_[4] = 3; indexData_[5] = 2;
	}
	else if (type == ParticleEmitter::Ring) {
		for (uint32_t index = 0; index < kRingDivide; index++) {
			indexData_[index * 6] = index * 4;
			indexData_[index * 6 + 1] = index * 4 + 1;
			indexData_[index * 6 + 2] = index * 4 + 2;
			indexData_[index * 6 + 3] = index * 4 + 1;
			indexData_[index * 6 + 4] = index * 4 + 3;
			indexData_[index * 6 + 5] = index * 4 + 2;
		}
	}
	indexResource_->Unmap(0, nullptr);
}

MyBase::Particle ParticleManager::CreateMoveParticle(std::mt19937& randomEngine, const MyBase::Vector3& position) {
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	MyBase::Particle particle{};
	particle.transform.scale = { 1.0f, 1.0f, 1.0f };
	particle.transform.rotate = { 0.0f, 0.0f, 0.0f };
	MyBase::Vector3 popPosition = position;
	particle.transform.translate = popPosition;
	particle.velocity = { distribution(randomEngine), distribution(randomEngine), distribution(randomEngine) };
	particle.color = { distribution(randomEngine), distribution(randomEngine), distribution(randomEngine), 1.0f };
	particle.lifeTime = 2.0f;
	return particle;
}

MyBase::Particle ParticleManager::CreateHitParticle(std::mt19937& randomEngine, const MyBase::Vector3& translate, ParticleEmitter::ParticleType type)
{
	std::uniform_real_distribution<float> distRotate(-std::numbers::pi_v<float>, std::numbers::pi_v<float>);
	std::uniform_real_distribution<float> distScale(1.0f, 3.0f);

	MyBase::Particle particle;

	if (type == ParticleEmitter::Box) {
		// 横に潰し、縦方向の大きさをランダムに入れる
		particle.transform.scale = { 0.08f, distScale(randomEngine), 1.0f };
		// ランダムに回転させる
		particle.transform.rotate = { 0.0f, 0.0f, distRotate(randomEngine) };
		
	}
	else if (type == ParticleEmitter::Ring) {
		particle.transform.scale = { 1.0f, 1.0f, 1.0f };
		particle.transform.rotate = { 0.0f, 0.0f, 0.0f };
	}
	particle.transform.translate = translate;
	particle.velocity = { 0.0f, 0.0f, 0.0f };			// 動かない
	particle.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	particle.lifeTime = 1.0f;
	particle.currentTime = 0;
	randomEngine;
	return particle;
}
