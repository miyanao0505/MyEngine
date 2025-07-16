#include "Skybox.h"
#include <CameraManager.h>
#include <LightManager.h>
#include <TextureManager.h>

// 初期化
void Skybox::Initislize(DirectXBase* dxBase, const std::string& filePath)
{
	// 引数で受け取ってメンバ変数に記録する
	dxBase_ = dxBase;
	textureFileName_ = filePath;

	// グラフィックスパイプラインの生成
	CreateGraphicsPipeline();

	// 頂点データの作成
	CreateVertexData();

	// マテリアルデータの作成
	CreateMaterialData();

	// 座標変換行列データの作成
	CreateTransformationMatrixData();

	// カメラデータの作成
	CreateCameraData();

	// 3DオブジェクトのTransformの初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture(textureFileName_);
}

// 更新
void Skybox::Update()
{
	// WorldMatrixの作成
	worldTransform_->UpdateWorldMatrix();
	MyBase::Matrix4x4 worldViewProjectionMatrix;
	if (CameraManager::GetInstance()->GetCamera()) {
		const MyBase::Matrix4x4& viewProjectionMatrix = CameraManager::GetInstance()->GetCamera()->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Matrix::Multiply(worldTransform_->GetWorldMatrix(), viewProjectionMatrix);
		cameraData_->worldPosition = CameraManager::GetInstance()->GetCamera()->GetTranslate();
	}
	else {
		worldViewProjectionMatrix = worldTransform_->GetWorldMatrix();
	}
	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = worldTransform_->GetWorldMatrix();
	transformationMatrixData_->WorldInverseTranspose = Matrix::Transpose(Matrix::Inverse(worldTransform_->GetWorldMatrix()));

}

// 描画
void Skybox::Draw()
{
	// WVP用のCBufferの場所を設定
	dxBase_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_.Get()->GetGPUVirtualAddress());
	// カメラ用のCBufferの場所を設定
	dxBase_->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResource_.Get()->GetGPUVirtualAddress());
	// ライトの設定
	//LightManager::GetInstance()->Draw(object3dBase_);

	// VBVの設定
	dxBase_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// マテリアルCBufferの場所を設定
	dxBase_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());
	// SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	dxBase_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureFileName_);
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	dxBase_->GetCommandList()->DrawInstanced(UINT(24), 1, 0, 0);

}

// ルートシグネチャの作成
void Skybox::CreateRootSignature()
{
	HRESULT hr;

	// DescriptorRange作成
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;														// 0から始まる
	descriptorRange[0].NumDescriptors = 1;															// 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;									// SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// Offsetを自動計算

	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter作成。複数設定できるので配列。
	D3D12_ROOT_PARAMETER rootParameters[7] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;					// CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					// PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;									// レジスタ番号0とバインド
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;					// CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;				// VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;									// レジスタ番号0とバインド
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					// PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;				// Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	// Tableで利用する数
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
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
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
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
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
	// 裏面(時計回り)を表示しない
	//rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// カリングしない(裏面も表示する)
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	D3D12_BLEND_DESC blendDesc{};
	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

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
	transformationMatrixResource_ = dxBase_->CreateBufferResource(sizeof(MyBase::TransformationMatrix));
	// 書き込むためのアドレスを取得
	transformationMatrixResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	// 単位行列を書き込んでおく
	transformationMatrixData_->WVP = Matrix::MakeIdentity4x4();
	transformationMatrixData_->World = Matrix::MakeIdentity4x4();
	transformationMatrixData_->WorldInverseTranspose = Matrix::MakeIdentity4x4();
}

// カメラデータ作成
void Skybox::CreateCameraData()
{
	// カメラ用のリソースを作る
	cameraResource_ = dxBase_->CreateBufferResource(sizeof(MyBase::CameraForGPU));
	// 書き込むためのアドレス取得
	cameraResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	cameraData_->worldPosition = { 0.0f, 0.0f, 0.0f };
	if (CameraManager::GetInstance()->GetCamera()) {
		CameraManager::GetInstance()->FindCamera("default");
		cameraData_->worldPosition = CameraManager::GetInstance()->GetCamera()->GetTranslate();
	}
}

// 頂点データ作成
void Skybox::CreateVertexData()
{
	// 頂点データをセットする
	// 右面。描画インデックスは[0,1,2][2,3,0]で内側を向く
	vertexData_[0].position =  {  1.0f,  1.0f,  1.0f,  1.0f };
	vertexData_[1].position =  {  1.0f,  1.0f, -1.0f,  1.0f };
	vertexData_[2].position =  {  1.0f, -1.0f,  1.0f,  1.0f };
	vertexData_[3].position =  {  1.0f, -1.0f, -1.0f,  1.0f };
	// 左面。描画インデックスは[4,5,6][6,5,7]
	vertexData_[4].position =  { -1.0f,  1.0f, -1.0f,  1.0f };
	vertexData_[5].position =  { -1.0f,  1.0f,  1.0f,  1.0f };
	vertexData_[6].position =  { -1.0f, -1.0f, -1.0f,  1.0f };
	vertexData_[7].position =  { -1.0f, -1.0f,  1.0f,  1.0f };
	// 前面。描画インデックスは[8,9,10][10,9,11]
	vertexData_[8].position =  { -1.0f,  1.0f,  1.0f,  1.0f };
	vertexData_[9].position =  {  1.0f,  1.0f,  1.0f,  1.0f };
	vertexData_[10].position = { -1.0f, -1.0f,  1.0f,  1.0f };
	vertexData_[11].position = {  1.0f, -1.0f,  1.0f,  1.0f };
	// 後面。描画インデックスは[12,13,14][14,13,15]
	vertexData_[12].position = {  1.0f,  1.0f, -1.0f,  1.0f };
	vertexData_[13].position = { -1.0f,  1.0f, -1.0f,  1.0f };
	vertexData_[14].position = {  1.0f, -1.0f, -1.0f,  1.0f };
	vertexData_[15].position = { -1.0f, -1.0f, -1.0f,  1.0f };
	// 上面。描画インデックスは[16,17,18][18,17,19]
	vertexData_[16].position = { -1.0f,  1.0f, -1.0f,  1.0f };
	vertexData_[17].position = {  1.0f,  1.0f, -1.0f,  1.0f };
	vertexData_[18].position = { -1.0f,  1.0f,  1.0f,  1.0f };
	vertexData_[19].position = {  1.0f,  1.0f,  1.0f,  1.0f };
	// 底面。描画インデックスは[20,21,22][22,21,23]
	vertexData_[20].position = { -1.0f, -1.0f,  1.0f,  1.0f };
	vertexData_[21].position = {  1.0f, -1.0f,  1.0f,  1.0f };
	vertexData_[22].position = { -1.0f, -1.0f, -1.0f,  1.0f };
	vertexData_[23].position = {  1.0f, -1.0f, -1.0f,  1.0f };

	// 頂点リソースを作る
	vertexResource_ = dxBase_->CreateBufferResource(sizeof(MyBase::ModelSkyboxVertexData) * size_t(24));
	// 頂点バッファビューを作成する
	vertexBufferView_.BufferLocation = vertexResource_.Get()->GetGPUVirtualAddress();		// リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(MyBase::ModelSkyboxVertexData) * size_t(24));		// 使用するリソースのサイズは頂点のサイズ
	vertexBufferView_.StrideInBytes = sizeof(MyBase::ModelSkyboxVertexData);						// 頂点あたりのサイズ

	// 頂点リソースにデータを書き込む
	vertexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));			// 書き込むためのアドレスを取得
}

// マテリアルデータ作成
void Skybox::CreateMaterialData()
{
	// マテリアル用のリソースを作る
	materialResource_ = dxBase_->CreateBufferResource(sizeof(MyBase::ModelMaterial));
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