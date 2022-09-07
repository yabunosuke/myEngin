#include "ConstantBufferManager.h"
#include <assert.h>

ConstantBufferManager *ConstantBufferManager::instance = nullptr;

ConstantBufferManager::ConstantBufferManager(ComPtr<ID3D12Device> dev)
{
    assert(instance == nullptr);
    
    constant_buffers_.resize(static_cast<int>(BufferName::MAX));
    AddConstantBuffer(sizeof(TestBuffer),BufferName::Test, dev); // test

    AddConstantBuffer(sizeof(CameraConstantBuffer),BufferName::Camera, dev);    // �J�����o�b�t�@
    AddConstantBuffer(sizeof(LightConstBufferData),BufferName::Light, dev);     // ���C�g�o�b�t�@


	instance = this;
}


bool ConstantBufferManager::AddConstantBuffer(size_t size, BufferName buffername,ComPtr<ID3D12Device> dev)
{
    ConstantBufferLapper buf;
    buf.CreateConstantBuffer(size, dev);
    constant_buffers_[static_cast<int>(buffername)] = buf;
    return false;
}
