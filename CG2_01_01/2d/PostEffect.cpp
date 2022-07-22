#include "PostEffect.h"
#include "PipelineManager.h"
#include "DirectXCommon.h"
#include "WinApp.h"
#include  <d3d12.h>

// 後でスプライトの継承をやめる

const float PostEffect::clear_color[4] = { 0.0f,0.0f,1.0f,1.0f };

PostEffect::PostEffect():
Sprite(
	0,
	{0,0},
	{ static_cast<float>(WinApp::windowWidth),static_cast<float>(WinApp::windowHeight) },
	{1,1,1,1},
	{0,0},
	false,
	false
)
{
}

void PostEffect::InitializePostEffect(ComPtr<ID3D12Device> dev)
{
	HRESULT result;

#pragma region 頂点バッファ

	//頂点バッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
#pragma endregion


	//頂点データ
	VertexPosUv vertices[4] = {
		{{-1.0f, -1.0f, 0.0f},{0.0f, 1.0f}}, //左下
		{{-1.0f, +1.0f, 0.0f},{0.0f, 0.0f}}, //左上
		{{+1.0f, -1.0f, 0.0f},{1.0f, 1.0f}}, //右下
		{{+1.0f, +1.0f, 0.0f},{1.0f, 0.0f}}, //右上
	};
	//頂点バッファへのデータ転送
	VertexPosUv *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView.StrideInBytes = sizeof(VertexPosUv);

	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));


#pragma region 画像生成
	// 塗りつぶした画像生成
	//リソース設定
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
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clear_color),
		IID_PPV_ARGS(&texture_buffer_));
	assert(SUCCEEDED(result));


	size_t texels = WinApp::windowWidth * WinApp::windowHeight;
	UINT *img = new UINT[texels];
	for (size_t i = 0; i < texels; ++i) {
		img[i] = 0xffffffff;
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

#pragma region 震度バッファ
	// 震度バッファ
	CD3DX12_RESOURCE_DESC depth_resource_desc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WinApp::windowWidth,
			WinApp::windowHeight,
			1,0,
			1,0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depth_resource_desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT,1.0f,0),
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

	//Spriteのイニシャライズ
	// 頂点バッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView.StrideInBytes = sizeof(VertexPosUv);

	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) {
		assert(0);
	}

	// 定数バッファにデータ転送
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->mat = matProjection;
		constBuff->Unmap(0, nullptr);
	}

}

void PostEffect::PreDrawScene(ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	//リソースバリアを変更(シェーダーリソースを描画可能に)
	cmd_list->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(texture_buffer_.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = descriputor_heap_RTV_->GetCPUDescriptorHandleForHeapStart();
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descriputor_heap_DSV_->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmd_list->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//ビューポートの設定
	cmd_list->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::windowWidth, WinApp::windowHeight));
	//シザリング矩形の設定
	cmd_list->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::windowWidth, WinApp::windowHeight));

	//全画面クリア
	cmd_list->ClearRenderTargetView(rtvH, clear_color, 0, nullptr);
	//深度バッファのクリア
	cmd_list->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void PostEffect::DrawPostEffect(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	static float time = 0.0f;
	time += 1.0f / 60.0f;
	if (time > 2.0f)time = 0.0f;

	//頂点バッファへのデータ転送
	HRESULT result = S_FALSE;
	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;

	// 頂点データ
	VertexPosUv vertices[4];
	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上
	
	D3D12_RESOURCE_DESC resDesc = texture_buffer_->GetDesc();

	float tex_left = texLeftTop.x / resDesc.Width;
	float tex_right = (texLeftTop.x + texSize.x) / resDesc.Width;
	float tex_top = texLeftTop.y / resDesc.Height;
	float tex_bottom = (texLeftTop.y + texSize.y) / resDesc.Height;

	vertices[LB].uv = { tex_left,	tex_bottom }; // 左下
	vertices[LT].uv = { tex_left,	tex_top }; // 左上
	vertices[RB].uv = { tex_right,	tex_bottom }; // 右下
	vertices[RT].uv = { tex_right,	tex_top }; // 右上


	// 頂点バッファへのデータ転送
	VertexPosUv *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// 定数バッファにデータ転送
	ConstBufferData *constMap = nullptr;
	result = this->constBuff->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = this->color;
		constMap->mat = matProjection;	// 行列の合成
		constMap->time = time / 2.0f;
		this->constBuff->Unmap(0, nullptr);
	}

	// パイプラインセット
	PipelineManager::GetInstance()->SetPipline(cmd_list, shader_name_);
	// プリミティブ形状を設定
	cmd_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// 頂点バッファの設定
	cmd_list->IASetVertexBuffers(0, 1, &this->vbView);
	ID3D12DescriptorHeap *ppHeaps[] = { descriputor_heap_SRV_.Get() };
	// デスクリプタヒープをセット
	cmd_list->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmd_list->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmd_list->SetGraphicsRootDescriptorTable(
		1, descriputor_heap_SRV_->GetGPUDescriptorHandleForHeapStart());
	// 描画コマンド
	cmd_list->DrawInstanced(4, 1, 0, 0);


}

void PostEffect::PostDrawScene(ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	cmd_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture_buffer_.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

}
