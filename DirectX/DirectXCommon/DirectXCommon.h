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
    //������
    static void Initialize();

    static bool DebugLayerON();             //DebugLayer��L���ɂ���
    static bool DXGIFactoryCreate();        //DXGI�t�@�N�g���[�𐶐�
    static bool CreateDevice();             //Device����
    static bool CreateCommandAllocator();   //�R�}���h�A���P�[�^�̐���
    static bool CreateCommandList();        //�R�}���h���X�g�̐���
    static bool CreateCommandQueue();       //�R�}���h�L���[�̐���
    static bool CreateSwapchainDesc();      //�X���b�v�`�F�[���̐���
    static bool CreateFence();              //�t�F���X�̐���
    static bool CreateDescriptorHeap();     //�f�X�N���v�^�[�q�[�v�̐���
    static bool CreateRenderTargetView();   //�����_�[�^�[�Q�b�g�r���[�̐���
    static bool CreateDepthBuffer();        //�[�x�o�b�t�@�̐���
    static bool CreateDepthView();        //�[�x�r���[�̐���
    //���t���[�������`��O
    static bool ResourceBarrierWriting();   //���\�[�X�o���A���������݉\��
    static bool ScreenClear();//��ʃN���A

    //���t���[�������`���
    static bool PlayCommandList();          //�R�}���h���X�g�̎��s�ƏI������

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
    static D3D12_HEAP_PROPERTIES deptHeapProp;   //�[�x�l�p�q�[�v�v���p�e�B
    static ComPtr<ID3D12Resource> depthBuffer;
    static D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    static ComPtr<ID3D12DescriptorHeap> dsvHeap;
    static CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH;
};

