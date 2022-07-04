#pragma once
#include <d3dx12.h>
#include <wrl.h>

class Texture
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	static HRESULT LoadTextureFromFile(ID3D12Device *dev,const wchar_t *filename,
		ID3D12DescriptorHeap **shader_resource_view);
};

