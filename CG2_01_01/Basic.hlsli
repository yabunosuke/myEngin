cbuffer cbuff0 : register(b0)
{
	float4 color;
	matrix mat;
}

struct VSOutput {
	float4 svpos : SV_POSITION;	//システム用長点座標
	float3 normal : NORMAL;	//法線ベクトル
	float2 uv : TEXCORD;	//uv値
};