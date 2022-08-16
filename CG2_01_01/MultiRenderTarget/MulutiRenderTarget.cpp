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


	// ���_�o�b�t�@����
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_));
	// ���_�f�[�^
	VertexPosUv vertices[4] = {
		{{-1.0f, -1.0f, 0.0f},{0.0f, 1.0f}}, //����
		{{-1.0f, +1.0f, 0.0f},{0.0f, 0.0f}}, //����
		{{+1.0f, -1.0f, 0.0f},{1.0f, 1.0f}}, //�E��
		{{+1.0f, +1.0f, 0.0f},{1.0f, 0.0f}}, //�E��
	};
	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv *vertex_map = nullptr;
	result = vertex_buffer_->Map(0, nullptr, (void **)&vertex_map);
	if (SUCCEEDED(result)) {
		memcpy(vertex_map, vertices, sizeof(vertices));
		vertex_buffer_->Unmap(0, nullptr);
	}
	// ���_�o�b�t�@�r���[�̍쐬
	vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vertex_buffer_view_.SizeInBytes = sizeof(VertexPosUv) * 4;
	vertex_buffer_view_.StrideInBytes = sizeof(VertexPosUv);


	// �萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constant_buffer_));
	assert(SUCCEEDED(result));

#pragma region �摜����

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texres_desc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			WinApp::windowWidth,
			static_cast<UINT>(WinApp::windowHeight),
			1, 0, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	for(int i = 0;i < buffer_count_;++i)
	{
		
		// �e�N�X�`���o�b�t�@����
		result = dev->CreateCommittedResource(	//GPU���\�[�X�̐���
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texres_desc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,	//�e�N�X�`���p�w��
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clear_color_),
			IID_PPV_ARGS(&texture_buffer_[i]));
		assert(SUCCEEDED(result));

		// �h��Ԃ����摜�𐶐�
		size_t texels = WinApp::windowWidth * WinApp::windowHeight;
		UINT *img = new UINT[texels];
		for (size_t j = 0; j < texels; ++j) {
			img[j] = 0xffffffff;	// ���F
		}

		// �e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
		result = texture_buffer_[i]->WriteToSubresource(
			0, nullptr,
			img, WinApp::windowWidth, WinApp::windowHeight
		);
		assert(SUCCEEDED(result));
		delete[] img;
	}

#pragma endregion

#pragma region SRV
	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc{};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 6;
	//SRV�f�X�N���v�^�q�[�v����
	result = dev->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descriputor_heap_SRV_));
	
	assert(SUCCEEDED(result));

	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};	//�ݒ�\����
	srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//RGBA
	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srv_desc.Texture1D.MipLevels = 1;

	//�V�F�[�_�[���\�[�X�r���[�쐬
	//dev->CreateShaderResourceView(
	//	texture_buffer_[0].Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
	//	&srv_desc,						//�e�N�X�`���ݒ���
	//	descriputor_heap_SRV_->GetCPUDescriptorHandleForHeapStart()
	//);
	for (int i = 0; i < buffer_count_; ++i)
	{
		//�V�F�[�_�[���\�[�X�r���[�쐬
		dev->CreateShaderResourceView(
			texture_buffer_[i].Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
			&srv_desc,						//�e�N�X�`���ݒ���
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
	rtv_descriptor_heap_desc.NumDescriptors = buffer_count_;	// �e�ʂ�����
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

#pragma region �[�x�o�b�t�@
	// �[�x�o�b�t�@
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

	//�f�X�N���v�^�q�[�v��DSV���쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; //�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depth_buffer_.Get(),
		&dsvDesc,
		descriputor_heap_DSV_->GetCPUDescriptorHandleForHeapStart());
#pragma endregion  


	// ���_�o�b�t�@����
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

	// ���_�o�b�t�@�r���[�̍쐬
	vertex_buffer_view_.BufferLocation = vertex_buffer_->GetGPUVirtualAddress();
	vertex_buffer_view_.SizeInBytes = sizeof(VertexPosUv) * 4;
	vertex_buffer_view_.StrideInBytes = sizeof(VertexPosUv);

	// �萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constant_buffer_));
	if (FAILED(result)) {
		assert(0);
	}

	// �萔�o�b�t�@�Ƀf�[�^�]��
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

	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[buffer_count_];
	for (int i = 0; i < buffer_count_; ++i)
	{
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descriputor_heap_RTV_->GetCPUDescriptorHandleForHeapStart(), i,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
			
	}

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descriputor_heap_DSV_->GetCPUDescriptorHandleForHeapStart();

	//�����_�[�^�[�Q�b�g���Z�b�g
	cmd_list->OMSetRenderTargets(buffer_count_, rtvHs, false, &dsvH);

	CD3DX12_VIEWPORT viewports[buffer_count_];
	CD3DX12_RECT scissor_rects[buffer_count_];
	for (int i = 0; i < buffer_count_; ++i)
	{
		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(WinApp::windowWidth), static_cast<float>(WinApp::windowHeight));
		scissor_rects[i] = CD3DX12_RECT(0, 0, WinApp::windowWidth, WinApp::windowHeight);
	}

	//�r���[�|�[�g�̐ݒ�
	cmd_list->RSSetViewports(buffer_count_,viewports);
	//�V�U�����O��`�̐ݒ�
	cmd_list->RSSetScissorRects(buffer_count_,scissor_rects);

	for (int i = 0; i < buffer_count_; ++i)
	{
		//�S��ʃN���A
		cmd_list->ClearRenderTargetView(rtvHs[i], clear_color_, 0, nullptr);
		
	}
	//�[�x�o�b�t�@�̃N���A
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

	//���_�o�b�t�@�ւ̃f�[�^�]��
	HRESULT result = S_FALSE;
	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f);
	float right = (1.0f) * static_cast<float>(WinApp::windowWidth);
	float top = (0.0f);
	float bottom = (1.0f) * static_cast<float>(WinApp::windowHeight);

	// ���_�f�[�^
	VertexPosUv vertices[4];
	vertices[LB].pos = { left,	bottom,	0.0f }; // ����
	vertices[LT].pos = { left,	top,	0.0f }; // ����
	vertices[RB].pos = { right,	bottom,	0.0f }; // �E��
	vertices[RT].pos = { right,	top,	0.0f }; // �E��

	D3D12_RESOURCE_DESC resDesc = texture_buffer_[0]->GetDesc();
	
	vertices[LB].uv = { 0,	1 }; // ����
	vertices[LT].uv = { 0,	0 }; // ����
	vertices[RB].uv = { 1,	1 }; // �E��
	vertices[RT].uv = { 1,	0 }; // �E��


	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv *vertMap = nullptr;
	result = vertex_buffer_->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertex_buffer_->Unmap(0, nullptr);
	}

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData *constMap = nullptr;
	result = this->constant_buffer_->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = {1,1,1,1};
		constMap->mat = DirectX::XMMatrixOrthographicOffCenterLH(
			0.0f, static_cast<float>(WinApp::windowWidth), static_cast<float>(WinApp::windowHeight), 0.0f, 0.0f, 1.0f);	// �s��̍���
		this->constant_buffer_->Unmap(0, nullptr);
	}

	
	// �p�C�v���C���Z�b�g
	PipelineManager::GetInstance()->SetPipline(cmd_list, "Deferred");
	// �v���~�e�B�u�`���ݒ�
	cmd_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// ���_�o�b�t�@�̐ݒ�
	cmd_list->IASetVertexBuffers(0, 1, &vertex_buffer_view_);
	// �f�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap *ppHeaps[] = {
		descriputor_heap_SRV_.Get()
	};
	cmd_list->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmd_list->SetGraphicsRootConstantBufferView(0, this->constant_buffer_->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g

	cmd_list->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descriputor_heap_SRV_->GetGPUDescriptorHandleForHeapStart(), 0,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);

	// �`��R�}���h
	cmd_list->DrawInstanced(4, 1, 0, 0);

}

void MulutiRenderTarget::PostDraw(ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	for (int i = 0; i < buffer_count_; ++i)
	{
		// ���\�[�X�o���A�̕ύX
		cmd_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture_buffer_[i].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}
}
