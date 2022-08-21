cbuffer CONSTANT_BUFFER : register(b0)
{
    matrix mat; //3D変換行列
}

// ライトの最大数
#define LIGHT_MAX 128

struct Light
{

	// ポイントライト用のデータ
	float4 position;	// ライトの座標
	float4 color;		// ライトの色
	float power;		// 強さ
	float range;		// 長さ
	bool is_active;		// 有効
	float PADING;		// パディング
};

cbuffer LIGHT_CONSTANT_BUFFER : register(b1)
{
	Light light[LIGHT_MAX];	//ポイントライト128個
}

// 頂点シェーダーからピクセルシェーダ―へのやり取りに使用する構造体
struct VSOutput {
	float4 svpos : SV_POSITION;	//システム用長点座標
	float2 texcoord : TEXCOORD;	//uv値
};