#include "Skybox.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "TextureManager.h"
#include "imgui.h"

#pragma region 定数
const size_t Skybox::kVertexCount = 24;
const size_t Skybox::kIndexCount = 36;

const float Skybox::kPi = 3.14159265359f;
const float Skybox::kImGuiDragSpeed = 0.01f;
const MyBase::ScopeF Skybox::kTranslateScope{ -100.0f, 100.0f };
const MyBase::ScopeF Skybox::kRotateScope{ -kPi, kPi };
const MyBase::ScopeF Skybox::kScaleScope{ 0.0f, 100.0f };

const float Skybox::kDefaultShininess = 40.8f;
const float Skybox::kDefaultReflectivity = 0.0f;
#pragma endregion

// 初期化
void Skybox::Initialize(const std::string& filePath, MyBase::Vector3 scale)
{
	// DirectXBaseの取得
	dxBase_ = DirectXBase::GetInstance();
	// 引数で受け取ってメンバ変数に記録する
	textureFileName_ = filePath;

	// グラフィックスパイプラインの生成
	CreateGraphicsPipeline();

	// 頂点データの作成
	CreateVertexData();

	// インデックスデータの作成
	CreateIndexData();

	// マテリアルデータの作成
	CreateMaterialData();

	// 座標変換行列データの作成
	CreateTransformationMatrixData();

	// カメラデータの作成
	CreateCameraData();

	// 3DオブジェクトのTransformの初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->SetScale(scale);	// スケールを設定

	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(textureFileName_);
}

// 更新
void Skybox::Update()
{
	// ワールド行列を更新
	worldTransform_->UpdateWorldMatrix();

	MyBase::Matrix4x4 worldViewProjectionMatrix;

	// カメラ情報の更新
	// 指定したカメラが存在する場合は WVP を作成
	if (CameraManager::GetInstance()->GetCamera()) {
		// WVP 行列の計算
		const MyBase::Matrix4x4& viewProjectionMatrix = CameraManager::GetInstance()->GetCamera()->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Matrix::Multiply(worldTransform_->GetWorldMatrix(), viewProjectionMatrix);
		
		// カメラのワールド座標をセット
		cameraMapped_->worldPosition = CameraManager::GetInstance()->GetCamera()->GetTranslate();
	}
	// 指定したカメラが存在しない場合はワールド行列のみ
	else {
		worldViewProjectionMatrix = worldTransform_->GetWorldMatrix();
	}
	
	// 座標変換行列データの更新
	transformationMatrixMapped_->WVP = worldViewProjectionMatrix;
	transformationMatrixMapped_->World = worldTransform_->GetWorldMatrix();
	transformationMatrixMapped_->WorldInverseTranspose = Matrix::Transpose(Matrix::Inverse(worldTransform_->GetWorldMatrix()));
}

// 描画
void Skybox::Draw()
{
	// 共通画面設定
	SetCommonScreen();

	// ルートパラメータ 1：座標変換行列(WVP)用のCBufferの場所を設定
	dxBase_->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootParam::kTransform), transformationMatrixBuffer_.Get()->GetGPUVirtualAddress());
	// ルートパラメータ 4：カメラ情報用のCBufferの場所を設定
	dxBase_->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootParam::kCamera), cameraBuffer_.Get()->GetGPUVirtualAddress());

	// VBVの設定
	dxBase_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IndexBufferViewを設定
	dxBase_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	// マテリアルCBufferの場所を設定
	dxBase_->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootParam::kMaterial), materialBuffer_.Get()->GetGPUVirtualAddress());
	// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	dxBase_->GetCommandList()->SetGraphicsRootDescriptorTable(static_cast<UINT>(RootParam::kTexture), TextureManager::GetInstance()->GetSrvHandleGPU(textureFileName_));
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。
	dxBase_->GetCommandList()->DrawIndexedInstanced(UINT(kIndexCount), 1, 0, 0, 0);
}

#ifdef _DEBUG
// デバック描画
void Skybox::DebugDraw()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader("Skybox"))
	{
		// Transformを一時変数として取得
		MyBase::Transform transform = GetTransform();

		// 移動
		ImGui::DragFloat3("Translate", &transform.translate.x, kImGuiDragSpeed, kTranslateScope.min, kTranslateScope.max);
		// 回転
		ImGui::DragFloat3("Rotate", &transform.rotate.x, kImGuiDragSpeed, kRotateScope.min, kRotateScope.max);
		// スケール
		ImGui::DragFloat3("Scale", &transform.scale.x, kImGuiDragSpeed, kScaleScope.min, kScaleScope.max);
		
		// 変更したTransformを適用
		SetTransform(transform);

		ImGui::Text("\n");
	}
	ImGui::PopID();
}
#endif // _DEBUG

// 描画に使用するテクスチャを変更
void Skybox::SetTexture(const std::string& textureName)
{
	// 使用するテクスチャ名を設定
	textureFileName_ = textureName;

	// 指定テクスチャをロード
	TextureManager::GetInstance()->LoadTexture(textureFileName_);
}

// ルートシグネチャの作成
void Skybox::CreateRootSignature()
{
	HRESULT hr;

	// DescriptorRange作成
	D3D12_DESCRIPTOR_RANGE descriptorRange[kDescriptorCount] = {};
	descriptorRange[0].BaseShaderRegister = 0;														// 0から始まる
	descriptorRange[0].NumDescriptors = 1;															// 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;									// SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// Offsetを自動計算

	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter作成。複数設定できるので配列。
	D3D12_ROOT_PARAMETER rootParameters[kRootParamCount] = {};
	// PixelShader 用 CBV(レジスタ b0)
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;					// CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					// PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;									// レジスタ番号0とバインド
	// VertexShader 用 CBV(レジスタ b0)
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;					// CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;				// VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;									// レジスタ番号0とバインド
	// PixelShader 用 SRV(テクスチャ)
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					// PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;				// Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	// Tableで利用する数
	// PixelShader 用の追加 CBV
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;					// CBVを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					// PixelShaderで使う
	rootParameters[3].Descriptor.ShaderRegister = 1;									// レジスタ番号1とバインド
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;					// CBVを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					// PixelShaderで使う
	rootParameters[4].Descriptor.ShaderRegister = 2;									// レジスタ番号2とバインド
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;					// CBVを使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					// PixelShaderで使う
	rootParameters[5].Descriptor.ShaderRegister = 3;									// レジスタ番号3とバインド
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;					// CBVを使う
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					// PixelShaderで使う
	rootParameters[6].Descriptor.ShaderRegister = 4;									// レジスタ番号4とバインド
	descriptionRootSignature.pParameters = rootParameters;					// ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);		// 配列の長さ

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[kSamplerCount] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;			// バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;		// 0～1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;		// 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;						// ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0;								// レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// PixelShaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	// シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr <ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		// エラー内容をログへ出力して強制停止
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	// バイナリを元に生成
	rootSignature_ = nullptr;
	hr = dxBase_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));
}

// グラフィックスパイプラインの生成
void Skybox::CreateGraphicsPipeline()
{
	HRESULT hr;

	// ルートシグネチャの作成
	CreateRootSignature();

	// InputLayer
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[kInputElementCount] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	// Shaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = dxBase_->CompileShader(L"resources/Shaders/Skybox.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = dxBase_->CompileShader(L"resources/Shaders/Skybox.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// カリングしない(裏面も表示する)
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// Blend設定
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// PipelineStateDesc設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();												// RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;													// InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };	// VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };	// PixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc;															// BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;													// RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定(気にしなくて良い)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// 比較はするのでDepth自体は有効
	depthStencilDesc.DepthEnable = true;
	// 全ピクセルがz=1に出力されるので、わざわざ書き込む必要がない
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	graphicsPipelineState_ = nullptr;
	hr = dxBase_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));
}

// 共通画面設定
void Skybox::SetCommonScreen()
{
	// ルートシグネチャをセットするコマンド
	dxBase_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
	// グラフィックスパイプラインステートをセットするコマンド
	dxBase_->GetCommandList()->SetPipelineState(graphicsPipelineState_.Get());
	// プリミティブトポロジーをセットするコマンド
	dxBase_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// 座標変換行列データ作成
void Skybox::CreateTransformationMatrixData()
{
	// TransformationMatrix用のリソースを作る
	transformationMatrixBuffer_ = dxBase_->CreateBufferResource(sizeof(MyBase::TransformationMatrix));
	// 書き込むためのアドレスを取得
	transformationMatrixBuffer_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixMapped_));
	// 単位行列を書き込んでおく
	transformationMatrixMapped_->WVP = Matrix::MakeIdentity4x4();
	transformationMatrixMapped_->World = Matrix::MakeIdentity4x4();
	transformationMatrixMapped_->WorldInverseTranspose = Matrix::MakeIdentity4x4();
}

// カメラデータ作成
void Skybox::CreateCameraData()
{
	// カメラ用のリソースを作る
	cameraBuffer_ = dxBase_->CreateBufferResource(sizeof(MyBase::CameraForGPU));
	// 書き込むためのアドレス取得
	cameraBuffer_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&cameraMapped_));
	cameraMapped_->worldPosition = { 0.0f, 0.0f, 0.0f };
	if (CameraManager::GetInstance()->GetCamera()) {
		CameraManager::GetInstance()->SetCamera("default");
		cameraMapped_->worldPosition = CameraManager::GetInstance()->GetCamera()->GetTranslate();
	}
}

// 頂点データ作成
void Skybox::CreateVertexData()
{
	// 頂点リソースを作る
	vertexBuffer_ = dxBase_->CreateBufferResource(sizeof(MyBase::ModelSkyboxVertexData) * kVertexCount);
	// 頂点バッファビューを作成する
	vertexBufferView_.BufferLocation = vertexBuffer_.Get()->GetGPUVirtualAddress();		// リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(MyBase::ModelSkyboxVertexData) * kVertexCount);		// 使用するリソースのサイズは頂点のサイズ
	vertexBufferView_.StrideInBytes = sizeof(MyBase::ModelSkyboxVertexData);						// 頂点あたりのサイズ

	// 頂点リソースにデータを書き込む
	vertexBuffer_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexMapped_));			// 書き込むためのアドレスを取得

	// 頂点データをセットする
	// 右面。描画インデックスは[0,1,2][2,1,3]で内側を向く
	vertexMapped_[0].position =  {  1.0f,  1.0f,  1.0f,  1.0f };
	vertexMapped_[1].position =  {  1.0f,  1.0f, -1.0f,  1.0f };
	vertexMapped_[2].position =  {  1.0f, -1.0f,  1.0f,  1.0f };
	vertexMapped_[3].position =  {  1.0f, -1.0f, -1.0f,  1.0f };
	// 左面。描画インデックスは[4,5,6][6,5,7]
	vertexMapped_[4].position =  { -1.0f,  1.0f, -1.0f,  1.0f };
	vertexMapped_[5].position =  { -1.0f,  1.0f,  1.0f,  1.0f };
	vertexMapped_[6].position =  { -1.0f, -1.0f, -1.0f,  1.0f };
	vertexMapped_[7].position =  { -1.0f, -1.0f,  1.0f,  1.0f };
	// 前面。描画インデックスは[8,9,10][10,9,11]
	vertexMapped_[8].position =  { -1.0f,  1.0f,  1.0f,  1.0f };
	vertexMapped_[9].position =  {  1.0f,  1.0f,  1.0f,  1.0f };
	vertexMapped_[10].position = { -1.0f, -1.0f,  1.0f,  1.0f };
	vertexMapped_[11].position = {  1.0f, -1.0f,  1.0f,  1.0f };
	// 後面。描画インデックスは[12,13,14][14,13,15]
	vertexMapped_[12].position = {  1.0f,  1.0f, -1.0f,  1.0f };
	vertexMapped_[13].position = { -1.0f,  1.0f, -1.0f,  1.0f };
	vertexMapped_[14].position = {  1.0f, -1.0f, -1.0f,  1.0f };
	vertexMapped_[15].position = { -1.0f, -1.0f, -1.0f,  1.0f };
	// 上面。描画インデックスは[16,17,18][18,17,19]
	vertexMapped_[16].position = { -1.0f,  1.0f, -1.0f,  1.0f };
	vertexMapped_[17].position = {  1.0f,  1.0f, -1.0f,  1.0f };
	vertexMapped_[18].position = { -1.0f,  1.0f,  1.0f,  1.0f };
	vertexMapped_[19].position = {  1.0f,  1.0f,  1.0f,  1.0f };
	// 底面。描画インデックスは[20,21,22][22,21,23]
	vertexMapped_[20].position = { -1.0f, -1.0f,  1.0f,  1.0f };
	vertexMapped_[21].position = {  1.0f, -1.0f,  1.0f,  1.0f };
	vertexMapped_[22].position = { -1.0f, -1.0f, -1.0f,  1.0f };
	vertexMapped_[23].position = {  1.0f, -1.0f, -1.0f,  1.0f };
}

// インデックスデータ作成
void Skybox::CreateIndexData()
{
	// インデックスリソースを作る
	indexBuffer_ = dxBase_->CreateBufferResource(sizeof(uint32_t) * kIndexCount);
	// インデックスバッファビューを作成する
	indexBufferView_.BufferLocation = indexBuffer_.Get()->GetGPUVirtualAddress();		// リソースの先頭のアドレスから使う
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * kIndexCount);				// 使用するリソースのサイズはインデックスのサイズ
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;										// インデックスはuint32_tとする
	// インデックスリソースにデータを書き込む
	indexBuffer_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&indexMapped_));			// 書き込むためのアドレスを取得
	// インデックスデータをセットする
	// 右面。描画インデックスは[0,1,2][2,1,3]で内側を向く
	indexMapped_[0] = 0; indexMapped_[1] = 1; indexMapped_[2] = 2;
	indexMapped_[3] = 2; indexMapped_[4] = 1; indexMapped_[5] = 3;
	// 左面。描画インデックスは[4,5,6][6,5,7]
	indexMapped_[6] = 4; indexMapped_[7] = 5; indexMapped_[8] = 6;
	indexMapped_[9] = 6; indexMapped_[10] = 5; indexMapped_[11] = 7;
	// 前面。描画インデックスは[8,9,10][10,9,11]
	indexMapped_[12] = 8; indexMapped_[13] = 9; indexMapped_[14] = 10;
	indexMapped_[15] = 10; indexMapped_[16] = 9; indexMapped_[17] = 11;
	// 後面。描画インデックスは[12,13,14][14,13,15]
	indexMapped_[18] = 12; indexMapped_[19] = 13; indexMapped_[20] = 14;
	indexMapped_[21] = 14; indexMapped_[22] = 13; indexMapped_[23] = 15;
	// 上面。描画インデックスは[16,17,18][18,17,19]
	indexMapped_[24] = 16; indexMapped_[25] = 17; indexMapped_[26] = 18;
	indexMapped_[27] = 18; indexMapped_[28] = 17; indexMapped_[29] = 19;
	// 底面。描画インデックスは[20,21,22][22,21,23]
	indexMapped_[30] = 20; indexMapped_[31] = 21; indexMapped_[32] = 22;
	indexMapped_[33] = 22; indexMapped_[34] = 21; indexMapped_[35] = 23;
}

// マテリアルデータ作成
void Skybox::CreateMaterialData()
{
	// マテリアル用のリソースを作る
	materialBuffer_ = dxBase_->CreateBufferResource(sizeof(MyBase::ModelMaterial));
	// 書き込むためのアドレスを取得
	materialBuffer_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialMapped_));
	// 白で読み込む
	materialMapped_->color = MyBase::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	// 単位行列で初期化
	materialMapped_->uvTransform = Matrix::MakeIdentity4x4();
	// 光沢度
	materialMapped_->shininess = 40.80f;
	// 反射強度
	materialMapped_->reflectivity = 0.0f;
	// Lightingを有効にする
	materialMapped_->enableLighting = true;
}