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
    DebugLayerON();           //�f�o�b�O���C���[���I��
    DXGIFactoryCreate();      //DXGI�t�@�N�g���[�̐���
    CreateDevice();           //�O���t�B�b�N�A�_�v�^�[�̑I���E����
    CreateCommandAllocator(); //�R�}���h�A���P�[�^�̐���
    CreateCommandList();      //�R�}���h���X�g�𐶐�
    CreateCommandQueue();     //�R�}���h�L���[�̐���
    CreateSwapchainDesc();    //�e��ݒ�����ăX���b�v�`�F�[���𐶐�
    CreateDescriptorHeap();   //�e��ݒ�����ăf�X�N���v�^�q�[�v�𐶐�
    CreateRenderTargetView(); //�����_�[�^�[�Q�b�g�r���[�̐���
    CreateDepthBuffer();      //�[�x�o�b�t�@
    CreateFence();            //�t�F���X�̐���


}

bool DirectXCommon::DebugLayerON()
{
#ifdef _DEBUG
    //�f�o�b�O���C���[���I����
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
    // DXGI�t�@�N�g���[�̐���
    result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
    // �A�_�v�^�[�̗񋓗p
    std::vector<ComPtr<IDXGIAdapter1>> adapters;
    // �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
    for (int i = 0;
        dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
        i++)
    {
        adapters.push_back(tmpAdapter); // ���I�z��ɒǉ�����
    }

    for (int i = 0; i < adapters.size(); i++)
    {
        DXGI_ADAPTER_DESC1 adesc;
        adapters[i]->GetDesc1(&adesc);  // �A�_�v�^�[�̏����擾

        // �\�t�g�E�F�A�f�o�C�X�����
        if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
        }

        std::wstring strDesc = adesc.Description;   // �A�_�v�^�[��
        // Intel UHD Graphics�i�I���{�[�h�O���t�B�b�N�j�����
        if (strDesc.find(L"Intel") == std::wstring::npos)
        {
            tmpAdapter = adapters[i];   // �̗p
            break;
        }
    }
    return true;
}

bool DirectXCommon::CreateDevice()
{
    // �Ή����x���̔z��
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
        // �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
        result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
        if (result == S_OK)
        {
            // �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
            featureLevel = levels[i];
            break;
        }
    }
	return true;
}

bool DirectXCommon::CreateCommandAllocator()
{

    // �R�}���h�A���P�[�^�𐶐�
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
    swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // �F���̏���
    swapchainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
    swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
    swapchainDesc.BufferCount = 2;  // �o�b�t�@�����Q�ɐݒ�
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
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
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
    heapDesc.NumDescriptors = 2;    // ���\�̂Q��
    DirectXCommon::dev->CreateDescriptorHeap(&heapDesc,
        IID_PPV_ARGS(&rtvHeaps));
    return true;
}

bool DirectXCommon::CreateRenderTargetView()
{
    for (int i = 0; i < 2; i++)
    {
        // �X���b�v�`�F�[������o�b�t�@���擾
        result = DirectXCommon::swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
        // �f�X�N���v�^�q�[�v�̃n���h�����擾
        CD3DX12_CPU_DESCRIPTOR_HANDLE handle = 
            CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
                i,  //�f�B�X�N���v�^�̔ԍ�
                DirectXCommon::dev->GetDescriptorHandleIncrementSize(heapDesc.Type)//�f�B�X�N���v�^1���̃T�C�Y
            );
        // �����_�[�^�[�Q�b�g�r���[�̐���
        DirectXCommon::dev->CreateRenderTargetView(
            backBuffers[i].Get(),
            nullptr,
            handle);
    }
    return true;
}

bool DirectXCommon::CreateDepthBuffer()
{
    //���\�[�X�ݒ�
    CD3DX12_RESOURCE_DESC depthResDesc =
        CD3DX12_RESOURCE_DESC::Tex2D(
            DXGI_FORMAT_D32_FLOAT,
            WinApp::windowWidth,
            WinApp::windowHeight,
            1,0,
            1,0,
            D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
        );
    
    //���\�[�X����
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
    dsvHeapDesc.NumDescriptors = 1;	//�[�x�r���[
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    result = DirectXCommon::dev->CreateDescriptorHeap(
        &dsvHeapDesc,
        IID_PPV_ARGS(&dsvHeap)
    );
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
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
    //�����_�[�^�[�Q�b�g���������݉\��
    bbIndex = swapchain->GetCurrentBackBufferIndex();
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    //�����_�[�^�[�Q�b�g�ݒ�
    rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
        bbIndex,
        dev->GetDescriptorHandleIncrementSize(DirectXCommon::heapDesc.Type)
    );
    //�[�x�X�e���V���r���[
    dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
    //�����_�[�^�[�Q�b�g�Z�b�g
    cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);


    //�r���[�|�[�g�̐ݒ�
    cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::windowWidth, WinApp::windowHeight));
    //�V�U�[��`�̐ݒ�
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
    //���\�[�X�o���A�ύX
    cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
    // ���߂̃N���[�Y
    cmdList->Close();
    // �R�}���h���X�g�̎��s
    ID3D12CommandList *cmdLists[] = { DirectXCommon::cmdList.Get() }; // �R�}���h���X�g�̔z��
    cmdQueue->ExecuteCommandLists(1, cmdLists);
    // �R�}���h���X�g�̎��s������҂�
    cmdQueue->Signal(fence.Get(), ++fenceVal);
    if (fence->GetCompletedValue() != fenceVal) {
        HANDLE event = CreateEvent(nullptr, false, false, nullptr);
        fence->SetEventOnCompletion(fenceVal, event);
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
    }

    cmdAllocator->Reset(); // �L���[���N���A
    cmdList->Reset(DirectXCommon::cmdAllocator.Get(), nullptr);  // �ĂуR�}���h���X�g�𒙂߂鏀��

    // �o�b�t�@���t���b�v�i���\�̓��ւ��j
    swapchain->Present(1, 0);
    
    return false;
}
