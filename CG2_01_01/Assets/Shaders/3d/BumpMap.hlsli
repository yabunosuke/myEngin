struct VSInput
{
	float3 position : POSITION;		// 位置座標
	float3 normal	: NOMARL;		// 法線ベクトル
	float3 texcoord : TEXCOORD;		// テクスチャ座標
	float3 tangent	: TANGENT;		// 接線ベクトル
};