#include "ImGuiManager.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

namespace
{
	constexpr DXGI_FORMAT kImGuiRTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
}

// 初期化
void ImGuiManager::Initialize(WindowsAPI* winBase, SrvManager* srvManager)
{
	// 引数をセット
	dxBase_ = DirectXBase::GetInstance();
	srvManager_ = srvManager;

	// ImGuiのコンテキストを生成
	ImGui::CreateContext();
	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();

	// win32用初期化
	ImGui_ImplWin32_Init(winBase->GetHwnd());

	// DirectX12用初期化
	ImGui_ImplDX12_Init(
		dxBase_->GetDevice(),
		static_cast<int>(dxBase_->GetBackBufferCount()),
		kImGuiRTVFormat, 
		srvManager_->GetDescriptorHeapForImGui(),
		srvManager_->GetDescriptorHeapForImGui()->GetCPUDescriptorHandleForHeapStart(),
		srvManager_->GetDescriptorHeapForImGui()->GetGPUDescriptorHandleForHeapStart()
	);
	srvManager_->Allocate();
}

// 終了
void ImGuiManager::Finalize()
{
	// 後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

// ImGui受付開始
void ImGuiManager::Begin()
{
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

// ImGui受付終了
void ImGuiManager::End()
{
	// 描画前準備
	ImGui::Render();
}

// 画面への描画
void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* commandList = dxBase_->GetCommandList();

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvManager_->GetDescriptorHeapForImGui() };
	commandList->SetDescriptorHeaps(kImGuiDescriptorHeapCount, ppHeaps);
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}
