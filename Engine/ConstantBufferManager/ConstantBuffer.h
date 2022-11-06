#pragma once
#include <DirectXMath.h>
struct SpriteDate {
	DirectX::XMFLOAT4 color; //色(RGBA)
	DirectX::XMMATRIX mat;	//3D変換行列
};

// 定数バッファ用データ構造体
struct TestBuffer {
	DirectX::XMMATRIX mat;	//3D変換行列

};

// ライトデータ
struct LightDate
{
	/// <summary>
	/// ライトの種類
	/// </summary>
	/// <param name="x = 1">Spot Light</param>
	/// <param name="y = 1">Directional Light</param>
	/// <param name="z = 1">Point Light</param>
	DirectX::XMFLOAT4 light_type	{ 0,0,1,0 };

	// 共通項目
	DirectX::XMFLOAT4 position		{ 0,0,0,0 };	// ライトの座標
	DirectX::XMFLOAT4 color			{ 1,1,1,1 };	// ライトの色

	float intensity					{ 1.0f };			// 輝度
	float range						{ 50.0f};			// 影響距離
	float is_active					{ 1.0f };			// 有効
	float PADING01					{ 0.0f };			// パディング

	
	// ディレクションライト用
	DirectX::XMFLOAT4 direction		{ 1.0f,1.0f,1.0f,1.0f};	// ライトの向き
};
static const int LIGHT_MAX = 128;
// ライトバッファ用データ構造体
struct LightConstBufferData {
	LightDate light[LIGHT_MAX];
	DirectX::XMFLOAT4 eye_pos{ 0,0,0,0 };
};
//
//struct CameraDeta
//{
//	DirectX::XMFLOAT4 view_position;
//	DirectX::XMMATRIX mat_view;
//	DirectX::XMMATRIX mat_projection;
//};
// カメラ用構造体
struct CameraConstantBuffer {
	DirectX::XMFLOAT4 view_position;
	DirectX::XMFLOAT4X4 view_projection;
	DirectX::XMFLOAT4X4 inv_view_projection;
};
