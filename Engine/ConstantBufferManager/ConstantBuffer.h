#pragma once
#include <DirectXMath.h>
struct SpriteDate {
	DirectX::XMFLOAT4 color; //�F(RGBA)
	DirectX::XMMATRIX mat;	//3D�ϊ��s��
};

// �萔�o�b�t�@�p�f�[�^�\����
struct TestBuffer {
	DirectX::XMMATRIX mat;	//3D�ϊ��s��

};

// ���C�g�f�[�^
struct LightDate
{
	/// <summary>
	/// ���C�g�̎��
	/// </summary>
	/// <param name="x = 1">Spot Light</param>
	/// <param name="y = 1">Directional Light</param>
	/// <param name="z = 1">Point Light</param>
	DirectX::XMFLOAT4 light_type	{ 0,0,1,0 };

	// ���ʍ���
	DirectX::XMFLOAT4 position		{ 0,0,0,0 };	// ���C�g�̍��W
	DirectX::XMFLOAT4 color			{ 1,1,1,1 };	// ���C�g�̐F

	float intensity					{ 1.0f };			// �P�x
	float range						{ 50.0f};			// �e������
	float is_active					{ 1.0f };			// �L��
	float PADING01					{ 0.0f };			// �p�f�B���O

	
	// �f�B���N�V�������C�g�p
	DirectX::XMFLOAT4 direction		{ 1.0f,1.0f,1.0f,1.0f};	// ���C�g�̌���
};
static const int LIGHT_MAX = 128;
// ���C�g�o�b�t�@�p�f�[�^�\����
struct LightConstBufferData {
	LightDate light[LIGHT_MAX];
	DirectX::XMFLOAT4 eye_pos{ 0,0,0,0 };
};
//
//struct CameraDeta
//{
//	DirectX::XMFLOAT4 view_position;
//	DirectX::XMMATRIX mat_view;
//	DirectX::XMMATRIX mat_projection;
//};
// �J�����p�\����
struct CameraConstantBuffer {
	DirectX::XMFLOAT4 view_position;
	DirectX::XMFLOAT4X4 view_projection;
	DirectX::XMFLOAT4X4 inv_view_projection;
};
