cbuffer cbuff0 : register(b0)
{
	float4 color;	//色(RGBA)
	matrix mat;		//3D変換行列
}

//頂点シェーダーからピクセルシェーダ―へのやり取りに使用する構造体
struct VSOutput {
	float4 svpos : SV_POSITION;	//システム用長点座標
	float2 uv : TEXCORD;	//uv値
};