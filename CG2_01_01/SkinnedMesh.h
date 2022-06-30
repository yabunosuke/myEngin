#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <fbxsdk.h>

struct Scene {
	// ノード
	struct Node {
		// ノードID
		uint64_t uniqueID = 0;
		// ノード名
		std::string name;
		// ノードタイプ
		FbxNodeAttribute::EType atribute = FbxNodeAttribute::EType::eUnknown;
		// 親のID
		int64_t pearentIndex = -1;
	};
	
	// ノードコンテナ
	std::vector<Node> nodes;

	/// <summary>
	/// ノードIDの検索
	/// </summary>
	/// <param name="uniqueID"></param>
	/// <returns>index</returns>
	int64_t indexof(uint64_t uniqueID) const {
		int64_t index = 0;
		for (const Node &node : nodes) {
			//IDが合致したらindexを返す
			if (node.uniqueID == uniqueID) {
				return index;
			}
			index++;
		}
		return -1;
	}
};


class SkinnedMesh
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;

public:	// サブクラス
	
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT3 texcoord;
	};

	// 定数
	struct Constants {
		XMFLOAT4X4 world;
		XMFLOAT4 materialColor;
	};

	struct Mesh
	{
		//メッシュID
		uint64_t uniqueID = 0;
		// メッシュ名
		std::string name;
		int64_t nodeIndex = 0;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

	private:
		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexBuffer;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
		// インデックスバッファ
		ComPtr<ID3D12Resource> indexBuffer;
		// インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW ibView = {};


		friend class SkinnedMesh;

	};
	std::vector<Mesh> meshes;

private:

	// 定数バッファ
	ComPtr<ID3D12Resource> constantBufferTransform;

public: 
	SkinnedMesh(ID3D12Device *dev, const char *fileName, bool trianglate = false);
	virtual ~SkinnedMesh() = default;

	void FetchMeshes(FbxScene *fbxScene, std::vector<Mesh> &meshes);

	void CreateComObjects(ID3D12Device *dev, const char *fileName);

	void Render(ComPtr<ID3D12GraphicsCommandList> cmdList, const XMFLOAT4X4 &world, const XMFLOAT4 &materialColor);

protected:

	Scene sceneView;
};

