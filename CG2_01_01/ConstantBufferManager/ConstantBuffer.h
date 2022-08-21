#pragma once
#include <DirectXMath.h>


// 定数バッファ用データ構造体
struct TestBuffer {
	DirectX::XMMATRIX mat;	//3D変換行列

};

// ライトデータ
struct Light
{
	// ポイントライト用のデータ
	DirectX::XMFLOAT4 position;	// ライトの座標
	DirectX::XMFLOAT4 color;		// ライトの色

	float power;		// 強さ
	float range;		// 長さ
	bool is_active;		// 有効
	float PADING01;		// パディング
};
static const int LIGHT_MAX = 128;
// ライトバッファ用データ構造体
struct LightConstBufferData {
	Light light[LIGHT_MAX];
};

// カメラ用構造体
struct CameraConstantBuffer {
	DirectX::XMFLOAT4 view_position;
	DirectX::XMFLOAT4X4 view_projection;
	DirectX::XMFLOAT4X4 inv_view_projection;
};

struct VertexPosUv
{
};