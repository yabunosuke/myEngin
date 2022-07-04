#include "Texture.h"
#include <DirectXTex.h>

using namespace DirectX;

HRESULT Texture::LoadTextureFromFile(ID3D12Device *dev, const wchar_t *filename, ID3D12DescriptorHeap **shader_resource_view)
{
    HRESULT result = S_OK;
 //   ComPtr<ID3D12Resource> resource;

 //   //auto it = resource.


	//result = LoadFromWICFile
	//(
	//	filename, WIC_FLAGS_NONE,
	//	&metadata, scratchImg
	//);



    return result;
}
