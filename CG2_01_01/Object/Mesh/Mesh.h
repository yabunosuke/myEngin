#pragma once
#include "Object/Object.h"

#include <DirectXMath.h>

#include <vector>
#include <wrl.h>

class Mesh : public Object
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;
	// IDはUINT64を使用する
	using NodeId = uint64_t;


public:
	//===========================================
	//
	//	  コンストラクタ
	//
	//===========================================
	Mesh();

	// 影響を受けるボーン数
	static const int MAX_BONE_INFLUENCES = 4;
	struct Vertex // 頂点情報
	{
		XMFLOAT4				position = { 0, 0, 0, 1 };		// 座標
		XMFLOAT3				normal = { 0, 0, 0 };			// 法線
		XMFLOAT3				tangent = { 0, 0, 0 };			// 接線
		XMFLOAT3				texcoord = { 0, 0 ,0 };			// 色情報
		XMFLOAT4				color = { 1, 1, 1, 1 };
		float bone_weights[MAX_BONE_INFLUENCES] = { 1,0,0,0 };
		uint32_t bone_indices[MAX_BONE_INFLUENCES];
	};


private:
	
	
	// 頂点情報
	std::vector<Vertex> vertices_;
	// メッシュコンテナ
	static std::vector<Mesh> meshs_;
};

