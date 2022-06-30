#include "Sprite.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCORD)
{
	VSOutput output;	//ピクセルシェーダーに渡す値
	output.svpos = mul(mat, pos);
	output.uv = uv;
	return output;
}