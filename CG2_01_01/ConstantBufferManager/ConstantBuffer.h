#pragma once
#include <DirectXMath.h>


// �萔�o�b�t�@�p�f�[�^�\����
struct TestBuffer {
	DirectX::XMMATRIX mat;	//3D�ϊ��s��

};

// ���C�g�f�[�^
struct Light
{
	// �|�C���g���C�g�p�̃f�[�^
	DirectX::XMFLOAT4 position;	// ���C�g�̍��W
	DirectX::XMFLOAT4 color;		// ���C�g�̐F

	float power;		// ����
	float range;		// ����
	bool is_active;		// �L��
	float PADING01;		// �p�f�B���O
};
static const int LIGHT_MAX = 128;
// ���C�g�o�b�t�@�p�f�[�^�\����
struct LightConstBufferData {
	Light light[LIGHT_MAX];
};

// �J�����p�\����
struct CameraConstantBuffer {
	DirectX::XMFLOAT4 view_position;
	DirectX::XMFLOAT4X4 view_projection;
	DirectX::XMFLOAT4X4 inv_view_projection;
};

struct VertexPosUv
{
};