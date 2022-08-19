#include "ConstantBufferLapper.h"
#include <d3dx12.h>
#include <assert.h>

bool ConstantBufferLapper::CreateConstantBuffer(size_t size, ComPtr<ID3D12Device> dev)
{
    HRESULT result = S_OK;

	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((size + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constant_buffer_));
	assert(!result);

	return true;
}
