#include "Image.h"

#include "DirectXCommon.h"
#include "ConstantBufferManager/ConstantBufferManager.h"


Image::Image():
MonoBehaviour("Image")
{
	HRESULT result = S_FALSE;
	// ���_�o�b�t�@����
	result = DirectXCommon::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertices();

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView.StrideInBytes = sizeof(VertexPosUv);

	mat_projection_ = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)WinApp::windowWidth, (float)WinApp::windowHeight, 0.0f, 0.0f, 1.0f);


	// �萔�o�b�t�@�̐���
	result = DirectXCommon::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpriteDate) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// �萔�o�b�t�@�Ƀf�[�^�]��
	SpriteDate *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->mat = mat_projection_;
		constBuff->Unmap(0, nullptr);
	}
	return;
}

void Image::TransferVertices()
{
	HRESULT result = S_FALSE;

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - pivot_.x) * width_;
	float right = (1.0f - pivot_.x) * width_;
	float top = (0.0f - pivot_.y) * height_;
	float bottom = (1.0f - pivot_.y) * height_;

	// ���_�f�[�^
	VertexPosUv vertices[4];
	vertices[LB].pos = { left,	bottom,	0.0f }; // ����
	vertices[LT].pos = { left,	top,	0.0f }; // ����
	vertices[RB].pos = { right,	bottom,	0.0f }; // �E��
	vertices[RT].pos = { right,	top,	0.0f }; // �E��

	// �e�N�X�`�����擾
	//if (TextureManager::texture_buffer_[texNumber]) {
	D3D12_RESOURCE_DESC resDesc = TextureManager::GetInstance()->GetTexture(texture_name_)->resource->GetDesc();

	float tex_left = /*texLeftTop.x /*/ resDesc.Width;
	float tex_right = (/*texLeftTop.x +*/ width_) / resDesc.Width;
	float tex_top = /*texLeftTop.y /*/ resDesc.Height;
	float tex_bottom = (/*texLeftTop.y +*/ height_) / resDesc.Height;

	vertices[LB].uv = { tex_left,	tex_bottom }; // ����
	vertices[LT].uv = { tex_left,	tex_top }; // ����
	vertices[RB].uv = { tex_right,	tex_bottom }; // �E��
	vertices[RT].uv = { tex_right,	tex_top }; // �E��
	//}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}
