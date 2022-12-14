#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")


class D3D12Base
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12Base();
	virtual ~D3D12Base();

	void Initialize(HWND hwbd);
	void Terminate();

	virtual void Render();

	virtual void Prepare() {};
	virtual void Cleanup() {};
	virtual void MakeCommand(ComPtr<ID3D12GraphicsCommandList> &command) { }

	// バッファリング数
	const UINT k_frame_buffer_count{ 2 };

protected:
	void DebugLayerOn();
	void CreateDXGIFactory();
	void CreateDevice();
	void CreateCommandQueue();
	void CreateSwapchain(HWND hwbd, int width, int height);

	void CreateDescriptorHeap();
	void CreateRenderTargetView();
	void CreateDepthBuffer(int width, int height);

	void CreateCommandAllocators();
	void CreateFrameFences();
	void CreateCommandList();

	ComPtr<ID3D12Device> device_;
	ComPtr<ID3D12CommandQueue> command_queue_;
	ComPtr<IDXGISwapChain4> swapchain_;

	ComPtr<IDXGIFactory3> factory_;
	ComPtr<IDXGIAdapter1> adapter_;

	ComPtr<ID3D12DescriptorHeap> heap_rtv_;
	ComPtr<ID3D12DescriptorHeap> heap_dsv_;

	std::vector<ComPtr<ID3D12Resource1>> render_targets_;
	ComPtr<ID3D12Resource1> depth_buffer_;

	CD3DX12_VIEWPORT viewport_;
	CD3DX12_RECT scissor_rect_;

	UINT rtv_descriptor_size_;
	UINT srvcbv_descriptor_size_;
	std::vector<ComPtr<ID3D12CommandAllocator>> command_allocators_;

	HANDLE fence_wait_event_;
	std::vector<ComPtr<ID3D12Fence1>> frame_fences_;
	std::vector<UINT64> frame_fence_values_;

	ComPtr<ID3D12GraphicsCommandList> command_list_;

	UINT frame_index_;
};

