#include "DirectXCommon.h"






HRESULT DirectXCommon::result;
ComPtr<ID3D12Device> DirectXCommon::dev;
ComPtr<IDXGIFactory6> DirectXCommon::dxgiFactory;
ComPtr<IDXGISwapChain4> DirectXCommon::swapchain;
ComPtr<ID3D12CommandAllocator> DirectXCommon::cmdAllocator;
ComPtr<ID3D12GraphicsCommandList> DirectXCommon::cmdList;
ComPtr<ID3D12CommandQueue> DirectXCommon::cmdQueue;
D3D12_DESCRIPTOR_HEAP_DESC DirectXCommon::heapDesc;
ComPtr<ID3D12DescriptorHeap> DirectXCommon::rtvHeaps;
ComPtr<IDXGIAdapter1> DirectXCommon::tmpAdapter;
ComPtr<ID3D12Fence> DirectXCommon::fence;
CD3DX12_CPU_DESCRIPTOR_HANDLE DirectXCommon::rtvH;
UINT64 DirectXCommon::fenceVal;

D3D12_RESOURCE_DESC DirectXCommon::depthResDesc;
D3D12_HEAP_PROPERTIES DirectXCommon::deptHeapProp;
ComPtr<ID3D12Resource> DirectXCommon::depthBuffer;
D3D12_DEPTH_STENCIL_VIEW_DESC DirectXCommon::dsvDesc;
D3D12_DESCRIPTOR_HEAP_DESC DirectXCommon::dsvHeapDesc;
ComPtr<ID3D12DescriptorHeap> DirectXCommon::dsvHeap;
CD3DX12_CPU_DESCRIPTOR_HANDLE DirectXCommon::dsvH;

std::vector<ComPtr<ID3D12Resource>> DirectXCommon::backBuffers(2);
UINT DirectXCommon::bbIndex;



void DirectXCommon::Initialize()
{
    DebugLayerON();           //デバッグレイヤーをオン
    DXGIFactoryCreate();      //DXGIファクトリーの生成
    CreateDevice();           //グラフィックアダプターの選択・生成
    CreateCommandAllocator(); //コマンドアロケータの生成
    CreateCommandList();      //コマンドリストを生成
    CreateCommandQueue();     //コマンドキューの生成
    CreateSwapchainDesc();    //各種設定をしてスワップチェーンを生成
    CreateDescriptorHeap();   //各種設定をしてデスクリプタヒープを生成
    CreateRenderTargetView(); //レンダーターゲットビューの生成
    CreateDepthBuffer();      //深度バッファ
    CreateFence();            //フェンスの生成


}

bool DirectXCommon::DebugLayerON()
{
#ifdef _DEBUG
    //デバッグレイヤーをオンに
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
    }
#endif
	return true;
}

bool DirectXCommon::DXGIFactoryCreate()
{
    // DXGIファクトリーの生成
    result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
    // アダプターの列挙用
    std::vector<ComPtr<IDXGIAdapter1>> adapters;
    // ここに特定の名前を持つアダプターオブジェクトが入る
    for (int i = 0;
        dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
        i++)
    {
        adapters.push_back(tmpAdapter); // 動的配列に追加する
    }

    for (int i = 0; i < adapters.size(); i++)
    {
        DXGI_ADAPTER_DESC1 adesc;
        adapters[i]->GetDesc1(&adesc);  // アダプターの情報を取得

        // ソフトウェアデバイスを回避
        if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
        }

        std::wstring strDesc = adesc.Description;   // アダプター名
        // Intel UHD Graphics（オンボードグラフィック）を回避
        if (strDesc.find(L"Intel") == std::wstring::npos)
        {
            tmpAdapter = adapters[i];   // 採用
            break;
        }
    }
    return true;
}

bool DirectXCommon::CreateDevice()
{
    // 対応レベルの配列
    D3D_FEATURE_LEVEL levels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    D3D_FEATURE_LEVEL featureLevel;

    for (int i = 0; i < _countof(levels); i++)
    {
        // 採用したアダプターでデバイスを生成
        result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
        if (result == S_OK)
        {
            // デバイスを生成できた時点でループを抜ける
            featureLevel = levels[i];
            break;
        }
    }
	return true;
}

bool DirectXCommon::CreateCommandAllocator()
{

    // コマンドアロケータを生成
    result = dev->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&cmdAllocator));
    return true;
}

bool DirectXCommon::CreateCommandList()
{
    result = dev->CreateCommandList(0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        cmdAllocator.Get(), nullptr,
        IID_PPV_ARGS(&cmdList));
    return true;
}

bool DirectXCommon::CreateCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
    dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
    return true;
}

bool DirectXCommon::CreateSwapchainDesc()
{
    DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
    swapchainDesc.Width = 1280;
    swapchainDesc.Height = 720;
    swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 色情報の書式
    swapchainDesc.SampleDesc.Count = 1; // マルチサンプルしない
    swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
    swapchainDesc.BufferCount = 2;  // バッファ数を２つに設定
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
    swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    ComPtr<IDXGISwapChain1> swapchain1;
    dxgiFactory->CreateSwapChainForHwnd(
        cmdQueue.Get(),
        WinApp::GetHwnd(),
        &swapchainDesc,
        nullptr,
        nullptr,
        &swapchain1);
    swapchain1.As(&swapchain);
    return true;
}

bool DirectXCommon::CreateFence()
{
    fence = nullptr;
    fenceVal = 0;
    result = DirectXCommon::dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    return true;
}

bool DirectXCommon::CreateDescriptorHeap()
{
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
    heapDesc.NumDescriptors = 2;    // 裏表の２つ
    DirectXCommon::dev->CreateDescriptorHeap(&heapDesc,
        IID_PPV_ARGS(&rtvHeaps));
    return true;
}

bool DirectXCommon::CreateRenderTargetView()
{
    for (int i = 0; i < 2; i++)
    {
        // スワップチェーンからバッファを取得
        result = DirectXCommon::swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
        // デスクリプタヒープのハンドルを取得
        CD3DX12_CPU_DESCRIPTOR_HANDLE handle = 
            CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
                i,  //ディスクリプタの番号
                DirectXCommon::dev->GetDescriptorHandleIncrementSize(heapDesc.Type)//ディスクリプタ1つ分のサイズ
            );
        // レンダーターゲットビューの生成
        DirectXCommon::dev->CreateRenderTargetView(
            backBuffers[i].Get(),
            nullptr,
            handle);
    }
    return true;
}

bool DirectXCommon::CreateDepthBuffer()
{
    //リソース設定
    CD3DX12_RESOURCE_DESC depthResDesc =
        CD3DX12_RESOURCE_DESC::Tex2D(
            DXGI_FORMAT_D32_FLOAT,
            WinApp::windowWidth,
            WinApp::windowHeight,
            1,0,
            1,0,
            D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
        );
    
    //リソース生成
    result = DirectXCommon::dev->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &depthResDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT,1.0f,0),
        IID_PPV_ARGS(&depthBuffer)
    );

    CreateDepthView();

    return true;
}

bool DirectXCommon::CreateDepthView()
{
    dsvHeapDesc.NumDescriptors = 1;	//深度ビュー
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    result = DirectXCommon::dev->CreateDescriptorHeap(
        &dsvHeapDesc,
        IID_PPV_ARGS(&dsvHeap)
    );
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    DirectXCommon::dev->CreateDepthStencilView(
        depthBuffer.Get(),
        &dsvDesc,
        dsvHeap->GetCPUDescriptorHandleForHeapStart()
    );
    return true;
}

bool DirectXCommon::ResourceBarrierWriting()
{
    //レンダーターゲットを書き込み可能に
    bbIndex = swapchain->GetCurrentBackBufferIndex();
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    //レンダーターゲット設定
    rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
        bbIndex,
        dev->GetDescriptorHandleIncrementSize(DirectXCommon::heapDesc.Type)
    );
    //深度ステンシルビュー
    dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
    //レンダーターゲットセット
    cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);


    //ビューポートの設定
    cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::windowWidth, WinApp::windowHeight));
    //シザー矩形の設定
    cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::windowWidth, WinApp::windowHeight));



    return true;
}



bool DirectXCommon::ScreenClear()
{
    float color[] = { 0.0f,0.0f,1.0f,0.0f };
    cmdList->ClearRenderTargetView(rtvH, color, 0, nullptr);
    cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
    return true;
}

bool DirectXCommon::PlayCommandList()
{
    //リソースバリア変更
    cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
    // 命令のクローズ
    cmdList->Close();
    // コマンドリストの実行
    ID3D12CommandList *cmdLists[] = { DirectXCommon::cmdList.Get() }; // コマンドリストの配列
    cmdQueue->ExecuteCommandLists(1, cmdLists);
    // コマンドリストの実行完了を待つ
    cmdQueue->Signal(fence.Get(), ++fenceVal);
    if (fence->GetCompletedValue() != fenceVal) {
        HANDLE event = CreateEvent(nullptr, false, false, nullptr);
        fence->SetEventOnCompletion(fenceVal, event);
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
    }

    cmdAllocator->Reset(); // キューをクリア
    cmdList->Reset(DirectXCommon::cmdAllocator.Get(), nullptr);  // 再びコマンドリストを貯める準備

    // バッファをフリップ（裏表の入替え）
    swapchain->Present(1, 0);
    
    return false;
}
