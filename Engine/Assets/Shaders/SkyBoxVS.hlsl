#include "SkyBox.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCORD)
{
	VSOutput output;	//�s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(mat, pos);
	output.uv = uv;
	return output;
}