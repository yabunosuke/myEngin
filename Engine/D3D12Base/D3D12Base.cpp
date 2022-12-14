#include "D3D12Base.h"
#include <exception>
#include <fstream>

D3D12Base::D3D12Base()
{
    render_targets_.resize(k_frame_buffer_count);
    frame_fence_values_.resize(k_frame_buffer_count);
}

D3D12Base::~D3D12Base()
{
}

void D3D12Base::Initialize(HWND hwnd)
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    DebugLayerOn();
    CreateDXGIFactory();
    CreateDevice();
    CreateCommandQueue();
    CreateSwapchain(hwnd, width, height);

    // デスクリプタ生成
    CreateDescriptorHeap();
    // レンダーターゲットビュー生成
    CreateRenderTargetView();
    // デプスバッファ生成
    CreateDepthBuffer(width, height);

    // コマンドアロケータ―
    CreateCommandAllocators();
    // フェンス
    CreateFrameFences();
    // コマンドリスト
    CreateCommandList();

    viewport_ = CD3DX12_VIEWPORT(0.0f, 0.0f, float(width), float(height));
    scissor_rect_ = CD3DX12_RECT(0, 0, LONG(width), LONG(height));

    Prepare();
}

void D3D12Base::Terminate()
{
}

void D3D12Base::Render()
{
}

void D3D12Base::DebugLayerOn()
{
#ifdef _DEBUG
    //デバッグレイヤーを有効化
    ComPtr<ID3D12Debug> debug_contriller;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_contriller))))
    {
        debug_contriller->EnableDebugLayer();

#if 0 // GBV を有効化する場合は1に
        ComPtr<ID3D12Debug3> debug3;
        debug.As(&debug3);
        if (debug3)
        {
            debug3->SetEnableGPUBasedValidation(true);
        }
#endif
    }
#endif
}

void D3D12Base::CreateDXGIFactory()
{
    HRESULT result{ S_FALSE };
	UINT dxgui_flags{ 0 };
    // DXGIファクトリーの生成
    result = CreateDXGIFactory2(dxgui_flags,IID_PPV_ARGS(&factory_));
    if (FAILED(result))
    {
        throw std::runtime_error("CreateDXGIFactory2 failed.");
    }

    // アダプターの列挙用
    UINT adapter_index{ 0 };
    ComPtr<IDXGIAdapter1> adapter;

    while (DXGI_ERROR_NOT_FOUND != factory_->EnumAdapters1(adapter_index, &adapter))
    {
        DXGI_ADAPTER_DESC1 desc{};
        adapter->GetDesc1(&desc);
        ++adapter_index;
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            continue;
        }
        // D3D12が使用可能か確認
        result = D3D12CreateDevice(
            adapter.Get(), D3D_FEATURE_LEVEL_12_0,
            __uuidof(ID3D12Device), nullptr);
        if (SUCCEEDED(result))
        {
            break;
        }
    }
    adapter.As(&adapter_);
}

void D3D12Base::CreateDevice()
{
    HRESULT result{ S_FALSE };
    // 対応レベルの配列
    D3D_FEATURE_LEVEL levels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    D3D_FEATURE_LEVEL feature_level;

    for (int i = 0; i < _countof(levels); i++)
    {
        // 採用したアダプターでデバイスを生成
        result = D3D12CreateDevice(adapter_.Get(), levels[i], IID_PPV_ARGS(&device_));
        if (result == S_OK)
        {
            // デバイスを生成できた時点でループを抜ける
            feature_level = levels[i];
            break;
        }
    }
}

void D3D12Base::CreateCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC command_queue_desc{};
    device_->CreateCommandQueue(&command_queue_desc, IID_PPV_ARGS(&command_queue_));
}

void D3D12Base::CreateSwapchain(HWND hwnd, int width, int height)
{
    // スワップチェイン生成
    DXGI_SWAP_CHAIN_DESC1 sc_desc{};
    sc_desc.BufferCount = k_frame_buffer_count;
    sc_desc.Width = width;
    sc_desc.Height = height;
    sc_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;            // 色情報の書式
    sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // バックバッファ用
    sc_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
    sc_desc.SampleDesc.Count = 1; // マルチサンプルしない

    ComPtr<IDXGISwapChain1> swapchain;
    factory_->CreateSwapChainForHwnd(
        command_queue_.Get(), hwnd,
        &sc_desc, nullptr, nullptr, &swapchain);
    
    swapchain.As(&swapchain_);
}

void D3D12Base::CreateDescriptorHeap()
{
    HRESULT result{ S_FALSE };

    // RTV のディスクリプタヒープ
    D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc{
      D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
      k_frame_buffer_count,
      D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
      0
    };
    result = device_->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&heap_rtv_));
    if (FAILED(result))
    {
        throw std::runtime_error("Failed CreateDescriptorHeap(RTV)");
    }
    rtv_descriptor_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // DSV のディスクリプタヒープ
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{
      D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
      1,
      D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
      0
    };
    result = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&heap_dsv_));
    if (FAILED(result))
    {
        throw std::runtime_error("Failed CreateDescriptorHeap(DSV)");
    }
}

void D3D12Base::CreateRenderTargetView()
{
    // スワップチェインイメージへのレンダーターゲットビュー生成
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(
        heap_rtv_->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < k_frame_buffer_count; ++i)
    {
        swapchain_->GetBuffer(i, IID_PPV_ARGS(&render_targets_[i]));
        device_->CreateRenderTargetView(render_targets_[i].Get(), nullptr, rtv_handle);
        // 参照するディスクリプタの変更
        rtv_handle.Offset(1, rtv_descriptor_size_);
    }
}

void D3D12Base::CreateDepthBuffer(int width, int height)
{
	HRESULT result{ S_FALSE };

    // デプスバッファの生成
    CD3DX12_RESOURCE_DESC depth_buffer_desc =
        CD3DX12_RESOURCE_DESC::Tex2D(
        DXGI_FORMAT_D32_FLOAT,
        width,
        height,
        1, 0,
        1, 0,
        D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
    );

    result = device_->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &depth_buffer_desc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
        IID_PPV_ARGS(&depth_buffer_)
    );

    if (FAILED(result))
    {
        throw std::runtime_error("Failed CreateCommittedResource(DepthBuffer)");
    }

    // デプスステンシルビュー生成
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc
    {
      DXGI_FORMAT_D32_FLOAT,  // Format
      D3D12_DSV_DIMENSION_TEXTURE2D,  // ViewDimension
      D3D12_DSV_FLAG_NONE,    // Flags
      { // D3D12_TEX2D_DSV
        0 // MipSlice
      }
    };
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsv_handle(heap_dsv_->GetCPUDescriptorHandleForHeapStart());
    device_->CreateDepthStencilView(
        depth_buffer_.Get(),
        &dsvDesc, dsv_handle);
}

void D3D12Base::CreateCommandAllocators()
{
    HRESULT result{ S_FALSE };
    command_allocators_.resize(k_frame_buffer_count);
    for (UINT i = 0; i < k_frame_buffer_count; ++i)
    {
        result = device_->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(&command_allocators_[i])
            );
        if (FAILED(result))
        {
            throw std::runtime_error("Failed CreateCommandAllocator");
        }
    }
}

void D3D12Base::CreateFrameFences()
{
	HRESULT result{ S_FALSE };
    frame_fences_.resize(k_frame_buffer_count);
    for (UINT i = 0; i < k_frame_buffer_count; ++i)
    {
        result = device_->CreateFence(
            0,  // 初期値
            D3D12_FENCE_FLAG_NONE,
            IID_PPV_ARGS(&frame_fences_[i]));
        if (FAILED(result))
        {
            throw std::runtime_error("Failed CreateFence");
        }
    }
}

void D3D12Base::CreateCommandList()
{
    HRESULT result{ S_FALSE };
    result = device_->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        command_allocators_[0].Get(), nullptr,
        IID_PPV_ARGS(&command_list_)
    );
    command_list_->Close();

}
