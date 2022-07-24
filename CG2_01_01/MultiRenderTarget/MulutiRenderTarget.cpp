#include "MulutiRenderTarget.h"
#include "WinAPP.h"

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

	// �e�N�X�`���o�b�t�@����
	result = dev->CreateCommittedResource(	//GPU���\�[�X�̐���
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texres_desc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,	//�e�N�X�`���p�w��
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clear_color_),
		IID_PPV_ARGS(&texture_buffer_));
	assert(SUCCEEDED(result));

	// �h��Ԃ����摜�𐶐�
	size_t texels = WinApp::windowWidth * WinApp::windowHeight;
	UINT *img = new UINT[texels];
	for (size_t i = 0; i < texels; ++i) {
		img[i] = 0xffffffff;	// ���F
	}

	// �e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	result = texture_buffer_->WriteToSubresource(
		0, nullptr,
		img, WinApp::windowWidth, WinApp::windowHeight
	);
	assert(SUCCEEDED(result));
	delete[] img;

#pragma endregion

#pragma region SRV
	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc{};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
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
	dev->CreateShaderResourceView(
		texture_buffer_.Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
		&srv_desc,						//�e�N�X�`���ݒ���
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
