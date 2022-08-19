cbuffer CONSTANT_BUFFER : register(b0)
{
    matrix mat; //3D�ϊ��s��
}

// ���C�g�̍ő吔
#define LIGHT_MAX 128
// ���s�����̍\����
//struct DirectionalLight
//{
//
//};
// �|�C���g���C�g�̍\����
struct PointLight
{
	float4 position;	// ���C�g�̍��W
	float4 color;		// ���C�g�̐F
	float power;		// ����
	float range;		// ����
	bool is_active;		// �L��
	float PADING;		// �p�f�B���O
};
// �X�|�b�g���C�g�̍\����
//struct SpotLight
//{
//
//};
// ���C�g�p�̃o�b�t�@
cbuffer LIGHT_CONSTANT_BUFFER : register(b1)
{
	PointLight point_light[LIGHT_MAX];	//�|�C���g���C�g128��
}

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�\�ւ̂����Ɏg�p����\����
struct VSOutput {
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
	float2 texcoord : TEXCOORD;	//uv�l
};