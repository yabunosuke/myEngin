// ���_�V�F�[�_�[�ɓ��͂����l
struct VSInput
{
	float4 position     : POSITION;	// ���[���h���W
	float3 normal		: NORMAL;	// �@��
	float3 tangent		: TANGENT;	// �ڐ�
	float3 texcoord		: TEXCOORD;	// UV���W
	float4 color		: COLOR;	// �F���
	float4 boneWeights : WEIGHTS;	// �{�[���e���x
	uint4  boneIndices : BONES;		// �{�[���C���f�b�N�X
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�Ɏ󂯓n�����l
struct VSOutput
{
	float4 viewprojection_position : SV_POSITION;	// VP���W
	float4 world_position : W_POSITION;	// ���[���h���W���W
	float2 texcoord : TEXCOORD;		// UV���W
	float4 color    : COLOR;		// �F���
	float3 normal	: NROMAL;		// �@��
	float3 tangent	: TANGENT;		// �ڐ�
	float3 binormal	: BINROMAL;		// �]�@��

};

// �}���`�����_�����O�p�\����
struct PSOutput
{
	float4 color				: SV_TARGET0;	// �F�摜
	float4 normal				: SV_TARGET1;	// �@���摜
	float4 metal_roughness		: SV_TARGET2;	// ���^���ƃ��t�l�X�摜
	float4 depth				: SV_TARGET3;	// �[�x�摜
	float4 position				: SV_TARGET4;	// ���[���h���W�摜
	float4 id					: SV_TARGET5;	// �����ύX�p�摜
};

// �J�����o�b�t�@
cbuffer CONSTANT_BUFFER_CAMERA : register(b0)
{
	float4 view_position;					// �J�������W
	row_major float4x4	view_projection;	// �r���[�v���W�F�N�V����
	row_major float4x4	inv_view_projection;// �r���[�v���W�F�N�V�����t�s��
};

// ���b�V���o�b�t�@
#define MAX_BONES 256
cbuffer CONSTANT_BUFFER_MESH : register(b1)
{
	row_major float4x4	bone_transforms[MAX_BONES];
};

// �T�u�Z�b�g�o�b�t�@
cbuffer CONSTANT_BUFFER_SUBSET : register(b2)
{
	float4 material_color;
	
};