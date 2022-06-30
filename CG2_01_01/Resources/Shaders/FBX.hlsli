
cbuffer cbuff0 : register(b0)
{
	matrix viewproj; //�r���[�v���W�F�N�V�����s��
	matrix world; //���[���h�s��
	float3 cameraPos; //�J�������W�i���[���h���W�j
};

//�{�[���̍ő吔
static const int MAX_BONES = 32;
//�{�[���̃X�L�j���O�s�񂪓���
cbuffer skinning : register(b3)
{
    matrix matSkinning[MAX_BONES];
};

//�o�[�e�b�N�X�o�b�t�@�[�̓���
struct VSInput
{
	float4 pos : POSITION;//�ʒu
	float3 normal : NORMAL;//���_�@��
	float2 uv : TEXCOORD;//�e�N�X�`���[���W
	uint4 boneIndices : BONEINDICES;//�{�[���ԍ�
	float4 boneWeights : BONEWEIGHTS;//�{�[���̃X�L���E�F�C�g
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION; //�V�X�e���p���_���W
	//float3 worldpos : POS; // ���[���h���W
	float3 normal :NORMAL; //�@��
	float2 uv  :TEXCOORD; //uv�l
};

//�X�L�j���O��̒��_�E�@��������
struct SkinOutput
{
    float4 pos;
    float3 normal;
};

// �}�e���A��
cbuffer cbuff1 : register(b1)
{
	//�A���x�h
    float3 baseColor;
	//�����x
    float metalness;
	//���ʔ��˓x
    float specular;
	//�e��
    float roughness;
	//�A���t�@�l
    float alpha;
}

// ���s�����̐�
static const int DIRLIGHT_NUM = 3;

struct DirLight
{
    float3 lightv; // ���C�g�ւ̕����̒P�ʃx�N�g��
    float3 lightcolor; // ���C�g�̐F(RGB)
    uint active;
};

// �_�����̐�
static const int POINTLIGHT_NUM = 3;

struct PointLight
{
    float3 lightpos; // ���C�g���W
    float3 lightcolor; // ���C�g�̐F(RGB)
    float3 lightatten; //���C�g���������W��
    uint active;
};

// �X�|�b�g���C�g�̐�
static const int SPOTLIGHT_NUM = 3;

struct SpotLight
{
    float3 lightv; // ���C�g�̌��������̋t�x�N�g���i�P�ʃx�N�g���j
    float3 lightpos; // ���C�g���W
    float3 lightcolor; // ���C�g�̐F(RGB)
    float3 lightatten; // ���C�g���������W��
    float2 lightfactoranglecos; // ���C�g�����p�x�̃R�T�C��
    uint active;
};

// �ۉe�̐�
static const int CIRCLESHADOW_NUM = 1;

struct CircleShadow
{
    float3 dir; // ���e�����̋t�x�N�g���i�P�ʃx�N�g���j
    float3 casterPos; // �L���X�^�[���W
    float distanceCasterLight; // �L���X�^�[�ƃ��C�g�̋���
    float3 atten; // ���������W��
    float2 factorAngleCos; // �����p�x�̃R�T�C��
    uint active;
};

cbuffer cbuff2 : register(b2)
{
    float3 ambientColor;
    DirLight dirLights[DIRLIGHT_NUM];
    PointLight pointLights[POINTLIGHT_NUM];
    SpotLight spotLights[SPOTLIGHT_NUM];
    CircleShadow circleShadows[CIRCLESHADOW_NUM];
}