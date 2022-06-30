#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

#include <vector>
#include <string>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include "WinApp.h"

//ComPtr
#include <wrl.h>
using namespace  Microsoft::WRL;

class DirectXCommon
{
public:
    //初期化
    static void Initialize();

    static bool DebugLayerON();             //DebugLayerを有効にする
    static bool DXGIFactoryCreate();        //DXGIファクトリーを生成
    static bool CreateDevice();             //Device生成
    static bool CreateCommandAllocator();   //コマンドアロケータの生成
    static bool CreateCommandList();        //コマンドリストの生成
    static bool CreateCommandQueue();       //コマンドキューの生成
    static bool CreateSwapchainDesc();      //スワップチェーンの生成
    static bool CreateFence();              //フェンスの生成
    static bool CreateDescriptorHeap();     //デスクリプターヒープの生成
    static bool CreateRenderTargetView();   //レンダーターゲットビューの生成
    static bool CreateDepthBuffer();        //深度バッファの生成
    static bool CreateDepthView();        //深度ビューの生成
    //毎フレーム処理描画前
    static bool ResourceBarrierWriting();   //リソースバリアを書き込み可能に
    static bool ScreenClear();//画面クリア

    //毎フレーム処理描画後
    static bool PlayCommandList();          //コマンドリストの実行と終了処理

    static ComPtr<ID3D12Device> dev;
//private:
    static HRESULT result;
    static ComPtr<IDXGIFactory6> dxgiFactory;
    static ComPtr<ID3D12DescriptorHeap> rtvHeaps;
    static ComPtr<IDXGISwapChain4> swapchain;
    static ComPtr<ID3D12CommandAllocator> cmdAllocator;
    static ComPtr<ID3D12GraphicsCommandList> cmdList;
    static ComPtr<ID3D12CommandQueue> cmdQueue;
    static D3D12_DESCRIPTOR_HEAP_DESC heapDesc;

    static ComPtr<IDXGIAdapter1> tmpAdapter;
    static ComPtr<ID3D12Fence> fence;
    static UINT64 fenceVal;

    static std::vector<ComPtr<ID3D12Resource>> backBuffers;
    static CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH;
    static UINT bbIndex;
    static D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
    static ComPtr<ID3D12DescriptorHeap> imguiDescHeap;

    static D3D12_RESOURCE_DESC depthResDesc;
    static D3D12_HEAP_PROPERTIES deptHeapProp;   //深度値用ヒーププロパティ
    static ComPtr<ID3D12Resource> depthBuffer;
    static D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    static ComPtr<ID3D12DescriptorHeap> dsvHeap;
    static CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH;
};

