#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

#include <vector>


class Mesh
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
	// サブクラス

	// 影響を受けるボーン数
	static const int MAX_BONE_INFLUENCES = 4;
	// 頂点データ構造体
	struct Vertex {
		XMFLOAT4				position = { 0, 0, 0, 1 };
		XMFLOAT3				normal = { 0, 0, 0 };
		XMFLOAT3				tangent = { 0, 0, 0 };
		XMFLOAT3				texcoord = { 0, 0 ,0 };
		XMFLOAT4				color = { 1, 1, 1, 1 };
		float bone_weights[MAX_BONE_INFLUENCES] = { 1,0,0,0 };
		uint32_t bone_indices[MAX_BONE_INFLUENCES];


	};



private:
	// メッシュ情報
	NodeId					id = 0;
	std::vector<Vertex>		vertices;				// 頂点
	std::vector<uint32_t>	indices;				// インデックス
	//std::vector<Subset>		subsets;				// サブセット

	// ボーン情報
	std::vector<uint32_t>	node_indices;			// ボーン
	std::vector<XMFLOAT4X4>	offset_transforms;		// ?


	// 頂点バッファ
	ComPtr<ID3D12Resource> vertexBuffer;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuffer;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	// メッシュ定数バッファ
	ComPtr<ID3D12Resource> mesh_constant_buffer_;
};

