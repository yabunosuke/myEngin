//�o�[�e�b�N�X�o�b�t�@�[�̓���
struct VSInput
{
    float4 position     : POSITION;     // �ʒu
    float4 normal       : NORMAL;       // ���_�@��
    float2 texcoord     : TEXCOORD;     // �e�N�X�`���[���W
    float4 bone_weights : WEIGHTS;      // �{�[���e���x
    uint4 bone_indices  : BONES;        // �{�[���C���f�b�N�X
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 position     : SV_POSITION;  // �V�X�e���p���_���W
    float4 worldPosition: POSITION;     // ���[���h���W
    float4 worldNormal  : NORMAL;       // �@��
    float2 texcoord     : TEXCOORD;     // uv�l
    float4 color        : COLOR;        // �F
};

cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    float4 materialColor;
};

cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 viewProjection;
    float4 lightDirection;
    float4 cameraPosition;
    
}
