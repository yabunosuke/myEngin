#include "DeferredShader.hlsli"
VSOutput main(float4 pos : POSITION, float2 texcoord : TEXCOORD)
{
	VSOutput output;	//�s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(mat, pos);
	output.texcoord = texcoord;
	return output;
}