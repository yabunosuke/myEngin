#include "imguiManager.h"
#include "DirectXCommon.h"
#include "WinAPP.h"

ComPtr<ID3D12DescriptorHeap> imguiManager::heap;

imguiManager *imguiManager::GetIns()
{
	static imguiManager *instans;
	return instans;
}

bool imguiManager::CreateDescriptorHeap()
{
	ComPtr<ID3D12DescriptorHeap> ret;
	
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	DirectXCommon::dev.Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(ret.ReleaseAndGetAddressOf()));

	heap = ret;
	
    return true;
}

bool imguiManager::Initialize()
{
	//�q�[�v����
	CreateDescriptorHeap();

	//ImGui������
	if (ImGui::CreateContext() == nullptr) {
		assert(0);
		return false;
	}
	auto &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	//�E�B���h�E�Y�p������
	bool blnResult = ImGui_ImplWin32_Init(WinApp::GetHwnd());
	if (!blnResult) {
		assert(0);
		return false;
	}

	//DirectX12�p������
	blnResult = ImGui_ImplDX12_Init(
		DirectXCommon::dev.Get(),	//DirectX12�f�o�C�X
		3,
		DXGI_FORMAT_R8G8B8A8_UNORM,	//�������ݐ��RTV�t�H�[�}�b�g
		heap.Get(),
		heap->GetCPUDescriptorHandleForHeapStart(),	//CPU�n���h��
		heap->GetGPUDescriptorHandleForHeapStart()	//GPU�n���h��
	);

	return true;
}


void imguiManager::Draw()
{
	ImGui::Render();
	DirectXCommon::cmdList->SetDescriptorHeaps(
		1,heap.GetAddressOf()
	);
	ImGui_ImplDX12_RenderDrawData(
		ImGui::GetDrawData(),
		DirectXCommon::cmdList.Get()
	);
}
