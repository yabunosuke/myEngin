#include "MulutiRenderTarget.h"
#include "WinAPP.h"

void MulutiRenderTarget::InitializeMulutiRenderTarget(ComPtr<ID3D12Device> dev)
{
	HRESULT result = S_OK;


	// 頂点バッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_));
	// 頂点データ
	VertexPosUv vertices[4] = {
		{{-1.0f, -1.0f, 0.0f},{0.0f, 1.0f}}, //左下
		{{-1.0f, +1.0f, 0.0f},{0.0f, 0.0f}}, //左上
		{{+1.0f, -1.0f, 0.0f},{1.0f, 1.0f}}, //右下
		{{+1.0f, +1.0f, 0.0f},{1.0f, 0.0f}}, //右上
	};
	// 頂点バッファへのデータ転送
	VertexPosUv *vertex_map = nullptr;
	result = vertex_buffer_->Map(0, nullptr, (void **)&vertex_map);
	if (SUCCEEDED(result)) {
		memcpy(vertex_map, vertices, sizeof(vertices));
		vertex_buffer_->Unmap(0, nullptr);
	}
	// 頂点バッファビューの作成
	vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vertex_buffer_view_.SizeInBytes = sizeof(VertexPosUv) * 4;
	vertex_buffer_view_.StrideInBytes = sizeof(VertexPosUv);


	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constant_buffer_));
	assert(SUCCEEDED(result));

#pragma region 画像生成

	// リソース設定
	CD3DX12_RESOURCE_DESC texres_desc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			WinApp::windowWidth,
			static_cast<UINT>(WinApp::windowHeight),
			1, 0, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	// テクスチャバッファ生成
	result = dev->CreateCommittedResource(	//GPUリソースの生成
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texres_desc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,	//テクスチャ用指定
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clear_color_),
		IID_PPV_ARGS(&texture_buffer_));
	assert(SUCCEEDED(result));

	// 塗りつぶした画像を生成
	size_t texels = WinApp::windowWidth * WinApp::windowHeight;
	UINT *img = new UINT[texels];
	for (size_t i = 0; i < texels; ++i) {
		img[i] = 0xffffffff;	// 白色
	}

	// テクスチャバッファへのデータ転送
	result = texture_buffer_->WriteToSubresource(
		0, nullptr,
		img, WinApp::windowWidth, WinApp::windowHeight
	);
	assert(SUCCEEDED(result));
	delete[] img;

#pragma endregion

#pragma region SRV
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc{};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRVデスクリプタヒープ生成
	result = dev->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descriputor_heap_SRV_));
	assert(SUCCEEDED(result));

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};	//設定構造体
	srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//RGBA
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srv_desc.Texture1D.MipLevels = 1;

	//シェーダーリソースビュー作成
	dev->CreateShaderResourceView(
		texture_buffer_.Get(),	//ビューと関連付けるバッファ
		&srv_desc,						//テクスチャ設定情報
		descriputor_heap_SRV_->GetCPUDescriptorHandleForHeapStart()
	);

#pragma endregion  

#pragma  region RTV
	// RTV
	D3D12_DESCRIPTOR_HEAP_DESC rtv_descriptor_heap_desc{};
	rtv_descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtv_descriptor_heap_desc.NumDescriptors = 1;
	result = dev->CreateDescriptorHeap(&rtv_descriptor_heap_desc, IID_PPV_ARGS(&descriputor_heap_RTV_));

	dev->CreateRenderTargetView(texture_buffer_.Get(), nullptr, descriputor_heap_RTV_->GetCPUDescriptorHandleForHeapStart());
#pragma endregion  

#pragma region 深度バッファ
	// 深度バッファ
	CD3DX12_RESOURCE_DESC depth_resource_desc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WinApp::windowWidth,
			WinApp::windowHeight,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depth_resource_desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depth_buffer_)
	);
#pragma endregion 

#pragma region DSV
	// DSV
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descHeapDesc.NumDescriptors = 1;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriputor_heap_DSV_));
	assert(SUCCEEDED(result));

	//デスクリプタヒープにDSVを作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; //深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depth_buffer_.Get(),
		&dsvDesc,
		descriputor_heap_DSV_->GetCPUDescriptorHandleForHeapStart());
#pragma endregion  


	// 頂点バッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_));
	if (FAILED(result)) {
		assert(0);
	}

	// 頂点バッファビューの作成
	vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vertex_buffer_view_.SizeInBytes = sizeof(VertexPosUv) * 4;
	vertex_buffer_view_.StrideInBytes = sizeof(VertexPosUv);

	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constant_buffer_));
	if (FAILED(result)) {
		assert(0);
	}

	// 定数バッファにデータ転送
	ConstBufferData *constMap = nullptr;
	result = constant_buffer_->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = {1,1,1,1};
		//constMap->mat = matProjection;
		constant_buffer_->Unmap(0, nullptr);
	}

}
