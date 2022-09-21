cbuffer CONSTANT_BUFFER : register(b0)
{
    matrix mat; //3D�ϊ��s��
}

// ���C�g�̍ő吔
#define LIGHT_MAX 128

struct Light
{
	/// <summary>
	/// ���C�g�̎��
	/// </summary>
	/// <param name="x = 1">Spot Light</param>
	/// <param name="y = 1">Directional Light</param>
	/// <param name="z = 1">Point Light</param>
	float4 light_type;

	// ���ʍ���
	float4 position;	// ���C�g�̍��W
	float4 color;		// ���C�g�̐F

	float intensity;	// ����
	float range;		// ����
	float is_active;	// �L��
	float PADING;		// �p�f�B���O

	// �f�B���N�V�������C�g�p
	float4 direction_;	// ���C�g�̌���
};

cbuffer LIGHT_CONSTANT_BUFFER : register(b1)
{
	Light light[LIGHT_MAX];	//�|�C���g���C�g128��
	float4 eye_pos;
}

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�\�ւ̂����Ɏg�p����\����
struct VSOutput {
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
	float2 texcoord : TEXCOORD;	//uv�l
};