#include "Texture.h"
#include <DirectXTex.h>

using namespace DirectX;

const int Texture::srv_count_;
std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> Texture::texture_buffer_;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Texture::descriptor_heap_;

void Texture::CreateDescriptorHeap(ID3D12Device *dev)
{
	HRESULT result = S_OK;

	//デスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = srv_count_;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptor_heap_));
	if (FAILED(result)) {
		assert(0);
	}
}

HRESULT Texture::LoadTextureFromFile(ID3D12Device *dev, const wchar_t *filename, ID3D12DescriptorHeap **shader_resource_view)
{
    HRESULT result = S_OK;

	//テクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		assert(0);
	}

	const Image *img = scratchImg.GetImage(0, 0, 0);

	//リソース設定
	CD3DX12_RESOURCE_DESC texresDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels);

	// 仮バッファ
	std::wstring ws(filename);
	std::string str(ws.begin(), ws.end());
	// テクスチャバッファ生成
	result = dev->CreateCommittedResource(	//GPUリソースの生成
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,	//テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texture_buffer_.emplace_back()));

	// テクスチャバッファへのデータ転送
	result = texture_buffer_.back().Get()->WriteToSubresource(
		0,
		nullptr,	//全領域へコピー
		img->pixels,	//元データアドレス
		(UINT)img->rowPitch,	//1ラインサイズ
		(UINT)img->slicePitch	//全サイズ
	);
	if (FAILED(result)) {
		assert(0);
	}


	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
	srvDesc.Format = metadata.format;	//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture1D.MipLevels = 1;

	//シェーダーリソースビュー作成
	dev->CreateShaderResourceView(
		texture_buffer_.back().Get(),	//ビューと関連付けるバッファ
		&srvDesc,	//テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descriptor_heap_->GetCPUDescriptorHandleForHeapStart(),
			texture_buffer_.size()-1,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);

    return result;
}
