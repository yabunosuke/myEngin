#include "Sprite.h"
#include "WinApp.h"
#include <DirectXTex.h>
#include "PipelineManager.h"
#include "oldTexture.h"
using namespace Microsoft::WRL;

//�ÓI�����o�̎���
XMMATRIX Sprite::matProjection;						//�ˉe�s��

bool Sprite::StaticInitialize(ID3D12Device *dev,int window_width, int window_height)
{
	//�ˉe�ϊ��s��v�Z
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);

	return true;
}


Sprite *Sprite::Create(ComPtr<ID3D12Device> dev,int texuer_num,XMFLOAT2 anchorpoint, bool isFlipX,bool isFlipY)
{
	HRESULT result = S_FALSE;

	// ���T�C�Y
	XMFLOAT2 size = { 100.0f, 100.0f };
	
	//�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	//if (TextureManager::texture_buffer_[texuer_num]) {
		//�e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = TextureManager::GetInstance()->GetResoureDesc(texuer_num);
		//�X�v���C�g�̑傫�����摜�̉𑜓x�ɍ��킹��
		size = { (float)resDesc.Width,(float)resDesc.Height };
	//}
	
	//�V�����X�v���C�g�����
	Sprite *sprite = new Sprite(texuer_num, {0,0}, size, { 1, 1, 1, 1 }, anchorpoint, isFlipX, isFlipY);
	if (sprite == nullptr) {
		return nullptr;
	}

	// ������
	if (!sprite->Initialize(dev)) {
		
		
		sprite;
		assert(0);
		return nullptr;
	}

	return sprite;
}

Sprite::Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY) {
	this->position = position;
	this->size = size;
	this->anchorpoint = anchorpoint;
	this->matWorld = XMMatrixIdentity();
	this->color = color;
	this->texNumber = texNumber;
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;
	this->texSize = size;
	this->isInvisible = false;
}

bool Sprite::Initialize(ComPtr<ID3D12Device> dev)
{
	HRESULT result = S_FALSE;

	// ���_�o�b�t�@����
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertices();

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView.StrideInBytes = sizeof(VertexPosUv);

	// �萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->mat = matProjection;
		constBuff->Unmap(0, nullptr);
	}
	return true;
}

void Sprite::SetRotation(float rotation) {
	this->rotation = rotation;

}

void Sprite::SetPosition(XMFLOAT2 position)
{
	this->position = position;

}

void Sprite::SetSize(XMFLOAT2 size)
{
	this->size = size;

}

void Sprite::SetScale(float scale)
{
	size.x *= scale;
	size.y *= scale;

}

void Sprite::SetColor(XMFLOAT4 color)
{
	this->color = color;
}

void Sprite::SetColor(float r, float g, float b, float a)
{
	this->color = { r, g, b, a };
}

void Sprite::SetColor(float color[4])
{
	this->color = { color[0],color[1],color[2],color[3] };
}

void Sprite::SetAnchorPoint(XMFLOAT2 anchorpoint) {
	this->anchorpoint = anchorpoint;

}

void Sprite::SetIsFlipX(bool isFlipX) {
	this->isFlipX = isFlipX;

}

void Sprite::SetIsFlipY(bool isFlipY) {
	this->isFlipY = isFlipY;

}

void Sprite::SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize) {
	this->texLeftTop = texBase;
	this->texSize = texSize;

}

void Sprite::Draw(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list, std::string pipeline_name)
{


	//���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertices();
	//�����Ȃ�`�悵�Ȃ�
	if (isInvisible) {
		return;
	}
	// ���[���h�s��̍X�V
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData *constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = this->color;
		constMap->mat = this->matWorld * matProjection;	// �s��̍���	
		this->constBuff->Unmap(0, nullptr);
	}

	// �p�C�v���C���Z�b�g
	PipelineManager::GetInstance()->SetPipline(cmd_list, pipeline_name);
	// �v���~�e�B�u�`���ݒ�
	cmd_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�o�b�t�@�̐ݒ�
	cmd_list->IASetVertexBuffers(0, 1, &this->vbView);


	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(cmd_list.Get(), 1, texNumber);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmd_list->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// �`��R�}���h
	cmd_list->DrawInstanced(4, 1, 0, 0);
}

void Sprite::TransferVertices()
{
	HRESULT result = S_FALSE;

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;
	if (isFlipX) {// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (isFlipY) {// �㉺����ւ�
		top = -top;
		bottom = -bottom;
	}

	// ���_�f�[�^
	VertexPosUv vertices[4];
	vertices[LB].pos = { left,	bottom,	0.0f }; // ����
	vertices[LT].pos = { left,	top,	0.0f }; // ����
	vertices[RB].pos = { right,	bottom,	0.0f }; // �E��
	vertices[RT].pos = { right,	top,	0.0f }; // �E��

	// �e�N�X�`�����擾
	//if (TextureManager::texture_buffer_[texNumber]) {
		D3D12_RESOURCE_DESC resDesc = TextureManager::GetInstance()->GetResoureDesc(texNumber);

		float tex_left = texLeftTop.x / resDesc.Width;
		float tex_right = (texLeftTop.x + texSize.x) / resDesc.Width;
		float tex_top = texLeftTop.y / resDesc.Height;
		float tex_bottom = (texLeftTop.y + texSize.y) / resDesc.Height;

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
