#include "Texture.h"
#include <DirectXTex.h>
#include <memory>

using namespace DirectX;



TextureManager *TextureManager::GetInstance()
{
	static TextureManager instance;
	return &instance;
}

uint32_t TextureManager::LoadTexture(ID3D12Device *dev, const wchar_t *filename)
{
	return TextureManager::GetInstance()->LoadTextureFromFile(dev, filename);
}


uint32_t TextureManager::MakeTexture(ID3D12Device *dev, DWORD color, DirectX::XMFLOAT2 size)
{
	return TextureManager::GetInstance()->MakeTextureFromColor(dev, color,size);
}

void TextureManager::SetGraphicsRootDescriptorTable(
	ID3D12GraphicsCommandList *cmd_list,
	UINT root_param_index,
	uint32_t texture_handle
)
{
	assert(texture_handle < textures_.size());
	ID3D12DescriptorHeap *pp_heaps[] = { descriptor_heap_.Get() };
	cmd_list->SetDescriptorHeaps(_countof(pp_heaps), pp_heaps);
	// シェーダリソースビューをセット
	cmd_list->SetGraphicsRootDescriptorTable(
		root_param_index, textures_[texture_handle].gpu_desc_handle_SRV);
}


void TextureManager::CreateDescriptorHeap(ID3D12Device *dev)
{
	HRESULT result = S_OK;

	descriptor_size_ = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//デスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = k_max_descriptor_;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptor_heap_));
	assert(SUCCEEDED(result));

	next_index_ = 0;

	for (size_t i = 0; i < k_max_descriptor_; i++) {
		textures_[i].resource.Reset();
		textures_[i].cpu_desc_handle_SRV.ptr = 0;
		textures_[i].gpu_desc_handle_SRV.ptr = 0;
		textures_[i].filename.clear();
	}
}
const D3D12_RESOURCE_DESC TextureManager::GetResoureDesc(uint32_t texture_handle)
{
	assert(texture_handle < textures_.size()); // ロードしていなければ注視
	Texture &texture = textures_.at(texture_handle);
	return texture.resource->GetDesc();
}


uint32_t TextureManager::LoadTextureFromFile(ID3D12Device *dev, const wchar_t *filename)
{
	// ロード数が規定値を超えるとエラー
	assert(next_index_ < k_max_descriptor_);
	uint32_t handle = next_index_;

	auto it = 
		std::find_if(
			textures_.begin(), 
			textures_.end(),
			[&](const auto &texture)
			{
				return texture.filename == filename;
			});
	// 画像が存在していればロードせずにハンドルを返す
	if (it != textures_.end()) {
		handle = static_cast<uint32_t>(std::distance(textures_.begin(), it));
		return handle;
	}



	Texture &texture = textures_.at(handle);
	texture.filename = filename;

	HRESULT result = S_FALSE;

	//テクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);
	assert(SUCCEEDED(result));

	const Image *img = scratchImg.GetImage(0, 0, 0);

	//リソース設定
	CD3DX12_RESOURCE_DESC texres_desc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heap_properties =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	// テクスチャバッファ生成
	result = dev->CreateCommittedResource(	//GPUリソースの生成
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&texres_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,	//テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texture.resource));
	assert(SUCCEEDED(result));

	// テクスチャバッファへのデータ転送
	result = texture.resource.Get()->WriteToSubresource(
		0,
		nullptr,	//全領域へコピー
		img->pixels,	//元データアドレス
		(UINT)img->rowPitch,	//1ラインサイズ
		(UINT)img->slicePitch	//全サイズ
	);
	assert(SUCCEEDED(result));


	//シェーダリソースビュー設定

	//D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};	//設定構造体
	//srv_desc.Format = metadata.format;	//RGBA
	//srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	//srv_desc.Texture1D.MipLevels = 1;


	texture.cpu_desc_handle_SRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		descriptor_heap_->GetCPUDescriptorHandleForHeapStart(), handle, descriptor_size_);
	texture.gpu_desc_handle_SRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		descriptor_heap_->GetGPUDescriptorHandleForHeapStart(), handle, descriptor_size_);


	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};	//設定構造体
	D3D12_RESOURCE_DESC resDesc = texture.resource->GetDesc();
	srv_desc.Format = resDesc.Format;	//RGBA
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srv_desc.Texture1D.MipLevels = 1;

	//シェーダーリソースビュー作成
	
	//dev->CreateShaderResourceView(
	//	texture_buffer_.back().Get(),	//ビューと関連付けるバッファ
	//	&srv_desc,	//テクスチャ設定情報
	//	CD3DX12_CPU_DESCRIPTOR_HANDLE(
	//		descriptor_heap_->GetCPUDescriptorHandleForHeapStart(),
	//		texture_buffer_.size()-1,
	//		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	//);

	dev->CreateShaderResourceView(
		texture.resource.Get(),
		&srv_desc,
		texture.cpu_desc_handle_SRV
	);

	++next_index_;

	return handle;
}

uint32_t TextureManager::MakeTextureFromColor(ID3D12Device *dev, DWORD color, XMFLOAT2 size)
{
	// ロード数が規定値を超えるとエラー
	assert(next_index_ < k_max_descriptor_);
	uint32_t handle = next_index_;

	std::wstring color_name { std::to_wstring(color) };
	auto it =
		std::find_if(
			textures_.begin(),
			textures_.end(),
			[&](const auto &texture)
			{
				return texture.filename == color_name;
			});
	// 画像が存在していればロードせずにハンドルを返す
	if (it != textures_.end()) {
		handle = static_cast<uint32_t>(std::distance(textures_.begin(), it));
		return handle;
	}



	Texture &texture = textures_.at(handle);
	texture.filename = color_name;

	HRESULT result = S_FALSE;

	//テクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	
	// 塗りつぶした画像生成
	size_t texels = size.x * size.y;
	UINT *img = new UINT[texels];
	for (size_t i = 0; i < texels; ++i) {
		img[i] = color;
	}


	//リソース設定
	CD3DX12_RESOURCE_DESC texres_desc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			size.x,
			static_cast<UINT>(size.y),
			1,0,1,0,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heap_properties =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	// テクスチャバッファ生成
	result = dev->CreateCommittedResource(	//GPUリソースの生成
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&texres_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,	//テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texture.resource));
	assert(SUCCEEDED(result));

	// テクスチャバッファへのデータ転送
	result = texture.resource.Get()->WriteToSubresource(
			0, nullptr,
		img,
		size.x,
		size.y
		);
	assert(SUCCEEDED(result));
	delete[] img;

	texture.cpu_desc_handle_SRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		descriptor_heap_->GetCPUDescriptorHandleForHeapStart(), handle, descriptor_size_);
	texture.gpu_desc_handle_SRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		descriptor_heap_->GetGPUDescriptorHandleForHeapStart(), handle, descriptor_size_);


	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};	//設定構造体
	D3D12_RESOURCE_DESC resDesc = texture.resource->GetDesc();
	srv_desc.Format = resDesc.Format;	//RGBA
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srv_desc.Texture1D.MipLevels = 1;

	//シェーダーリソースビュー作成
	dev->CreateShaderResourceView(
		texture.resource.Get(),
		&srv_desc,
		texture.cpu_desc_handle_SRV
	);

	++next_index_;

	return handle;
}
