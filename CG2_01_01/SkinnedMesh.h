#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <fbxsdk.h>
#include <unordered_map>

//#include "Component.h"

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
	// 頂点データ構造体
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT3 texcoord;
	};

	// メッシュバッファ用
	struct MeshConstantBuffer {
		XMFLOAT4X4 world;
		XMFLOAT4 materialColor;
	};

	// シーンバッファ用
	struct SceneConstantBuffer {
		XMFLOAT4X4 view_projection;
		XMFLOAT4 light_direction;
		XMFLOAT4 camera_position;
	};

	//// マテリアル
	//struct Material {
	//	// ID
	//	uint64_t unique_id;
	//	// 名前
	//	std::string name;
	//	
	//	XMFLOAT4 Ka = {0.2f,0.2f,0.2f,1.0f};	// Albedo
	//	XMFLOAT4 Kd = {0.8f,0.8f,0.8f,1.0f};	// Diffuse
	//	XMFLOAT4 Ks = {1.0f,1.0f,1.0f,1.0f};	// Specular
	//	
	//	// テクスチャの名前
	//	std::string texture_filenames[4];
	//	// シェーダーリソースビュー
	//	ComPtr<ID3D12DescriptorHeap> shader_resource_views[4];
	//};
	//std::unordered_map<uint64_t, Material> materials;
	
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


public:
	static void CreatePipline();


public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="fileName">リソースのファイル名</param>
	/// <param name="trianglate">三角形化の有無</param>
	SkinnedMesh(
		ID3D12Device *dev,
		const char *fileName,
		bool trianglate = true);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SkinnedMesh() = default;

	/// <summary>
	/// ノードからメッシュ情報を取得
	/// </summary>
	/// <param name="fbxScene">シーン</param>
	/// <param name="meshes">メッシュの格納先</param>
	void FetchMeshes(FbxScene *fbx_scene, std::vector<Mesh> &meshes);

	/// <summary>
	/// materialの取得
	/// </summary>
	/// <param name="fbx_scene">シーン</param>
	/// <param name="materials">materialの格納先</param>
	//void FetchMaterial(FbxScene *fbx_scene, std::unordered_map<uint64_t, Material> &materials);

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="fileName">ファイル名</param>
	void CreateComObjects(ID3D12Device *dev, const char *fileName);

	/// <summary>
	/// 描画コマンド発行
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="world">ワールド行列</param>
	/// <param name="materialColor">マテリアルカラー</param>
	void Render(ComPtr<ID3D12GraphicsCommandList> cmdList, const XMFLOAT4X4 &world, const XMFLOAT4 &materialColor);


private:
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature_;
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate_;
	// メッシュ定数バッファ
	ComPtr<ID3D12Resource> mesh_constant_buffer_;
	// シーン定数バッファ
	ComPtr<ID3D12Resource> scene_constant_buffer_;


protected:
	// シーンビュー
	Scene scene_view_;
};

