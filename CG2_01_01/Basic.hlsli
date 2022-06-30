cbuffer cbuff0 : register(b0)
{
	float4 color;
	matrix mat;
}

struct VSOutput {
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
	float3 normal : NORMAL;	//�@���x�N�g��
	float2 uv : TEXCORD;	//uv�l
};