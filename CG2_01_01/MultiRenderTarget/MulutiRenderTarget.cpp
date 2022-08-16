#include "MulutiRenderTarget.h"
#include "WinAPP.h"
#include  "PipelineManager.h"

#include "KeyboardInput.h"
#include "DirectXCommon.h"

const int MulutiRenderTarget::buffer_count_;

MulutiRenderTarget::MulutiRenderTarget()
{
}

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

	for(int i = 0;i < buffer_count_;++i)
	{
		
		// テクスチャバッファ生成
		result = dev->CreateCommittedResource(	//GPUリソースの生成
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texres_desc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,	//テクスチャ用指定
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clear_color_),
			IID_PPV_ARGS(&texture_buffer_[i]));
		assert(SUCCEEDED(result));

		// 塗りつぶした画像を生成
		size_t texels = WinApp::windowWidth * WinApp::windowHeight;
		UINT *img = new UINT[texels];
		for (size_t j = 0; j < texels; ++j) {
			img[j] = 0xffffffff;	// 白色
		}

		// テクスチャバッファへのデータ転送
		result = texture_buffer_[i]->WriteToSubresource(
			0, nullptr,
			img, WinApp::windowWidth, WinApp::windowHeight
		);
		assert(SUCCEEDED(result));
		delete[] img;
	}

#pragma endregion

#pragma region SRV
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc{};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 6;
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
	//dev->CreateShaderResourceView(
	//	texture_buffer_[0].Get(),	//ビューと関連付けるバッファ
	//	&srv_desc,						//テクスチャ設定情報
	//	descriputor_heap_SRV_->GetCPUDescriptorHandleForHeapStart()
	//);
	for (int i = 0; i < buffer_count_; ++i)
	{
		//シェーダーリソースビュー作成
		dev->CreateShaderResourceView(
			texture_buffer_[i].Get(),	//ビューと関連付けるバッファ
			&srv_desc,						//テクスチャ設定情報
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descriputor_heap_SRV_->GetCPUDescriptorHandleForHeapStart() ,i,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			)
		);
	}

#pragma endregion  

#pragma  region RTV
	// RTV
	D3D12_DESCRIPTOR_HEAP_DESC rtv_descriptor_heap_desc{};
	rtv_descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtv_descriptor_heap_desc.NumDescriptors = buffer_count_;	// 容量を二つ分に
	result = dev->CreateDescriptorHeap(&rtv_descriptor_heap_desc, IID_PPV_ARGS(&descriputor_heap_RTV_));
	assert(SUCCEEDED((result)));

	for (int i = 0; i < buffer_count_; ++i)
	{
		dev->CreateRenderTargetView(
			texture_buffer_[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descriputor_heap_RTV_->GetCPUDescriptorHandleForHeapStart(), i,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))

			);
		
	}
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

void MulutiRenderTarget::PreDrawScene(ComPtr<ID3D12Device> dev,ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	for(int i = 0;i<buffer_count_;++i)
	{
		cmd_list->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(texture_buffer_[i].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET));

	}

	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[buffer_count_];
	for (int i = 0; i < buffer_count_; ++i)
	{
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descriputor_heap_RTV_->GetCPUDescriptorHandleForHeapStart(), i,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
			
	}

	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descriputor_heap_DSV_->GetCPUDescriptorHandleForHeapStart();

	//レンダーターゲットをセット
	cmd_list->OMSetRenderTargets(buffer_count_, rtvHs, false, &dsvH);

	CD3DX12_VIEWPORT viewports[buffer_count_];
	CD3DX12_RECT scissor_rects[buffer_count_];
	for (int i = 0; i < buffer_count_; ++i)
	{
		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(WinApp::windowWidth), static_cast<float>(WinApp::windowHeight));
		scissor_rects[i] = CD3DX12_RECT(0, 0, WinApp::windowWidth, WinApp::windowHeight);
	}

	//ビューポートの設定
	cmd_list->RSSetViewports(buffer_count_,viewports);
	//シザリング矩形の設定
	cmd_list->RSSetScissorRects(buffer_count_,scissor_rects);

	for (int i = 0; i < buffer_count_; ++i)
	{
		//全画面クリア
		cmd_list->ClearRenderTargetView(rtvHs[i], clear_color_, 0, nullptr);
		
	}
	//深度バッファのクリア
	cmd_list->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void MulutiRenderTarget::DrawRenderTarget(ComPtr<ID3D12GraphicsCommandList> cmd_list, ComPtr<ID3D12Device> dev)
{
	static int tex = 0;
	if(KeyboardInput::GetIns()->GetKeyPressT(DIK_1))
	{
		tex = (tex + 1) % 6;
		
	}


	static float time = 0.0f;
	time += 1.0f / 60.0f;
	if (time > 2.0f)time = 0.0f;

	//頂点バッファへのデータ転送
	HRESULT result = S_FALSE;
	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f);
	float right = (1.0f) * static_cast<float>(WinApp::windowWidth);
	float top = (0.0f);
	float bottom = (1.0f) * static_cast<float>(WinApp::windowHeight);

	// 頂点データ
	VertexPosUv vertices[4];
	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上

	D3D12_RESOURCE_DESC resDesc = texture_buffer_[0]->GetDesc();
	
	vertices[LB].uv = { 0,	1 }; // 左下
	vertices[LT].uv = { 0,	0 }; // 左上
	vertices[RB].uv = { 1,	1 }; // 右下
	vertices[RT].uv = { 1,	0 }; // 右上


	// 頂点バッファへのデータ転送
	VertexPosUv *vertMap = nullptr;
	result = vertex_buffer_->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertex_buffer_->Unmap(0, nullptr);
	}

	// 定数バッファにデータ転送
	ConstBufferData *constMap = nullptr;
	result = this->constant_buffer_->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = {1,1,1,1};
		constMap->mat = DirectX::XMMatrixOrthographicOffCenterLH(
			0.0f, static_cast<float>(WinApp::windowWidth), static_cast<float>(WinApp::windowHeight), 0.0f, 0.0f, 1.0f);	// 行列の合成
		this->constant_buffer_->Unmap(0, nullptr);
	}

	
	// パイプラインセット
	PipelineManager::GetInstance()->SetPipline(cmd_list, "Deferred");
	// プリミティブ形状を設定
	cmd_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// 頂点バッファの設定
	cmd_list->IASetVertexBuffers(0, 1, &vertex_buffer_view_);
	// デスクリプタヒープをセット
	ID3D12DescriptorHeap *ppHeaps[] = {
		descriputor_heap_SRV_.Get()
	};
	cmd_list->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmd_list->SetGraphicsRootConstantBufferView(0, this->constant_buffer_->GetGPUVirtualAddress());
	// シェーダリソースビューをセット

	cmd_list->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descriputor_heap_SRV_->GetGPUDescriptorHandleForHeapStart(), 0,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);

	// 描画コマンド
	cmd_list->DrawInstanced(4, 1, 0, 0);

}

void MulutiRenderTarget::PostDraw(ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	for (int i = 0; i < buffer_count_; ++i)
	{
		// リソースバリアの変更
		cmd_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture_buffer_[i].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}
}
