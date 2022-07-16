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
	// ボーンの影響数
	static const int MAX_BONE_INFLUENCES = 4;
	// 頂点データ構造体
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT3 texcoord;
		float bone_weights[MAX_BONE_INFLUENCES] = { 1,0,0,0 };
		uint32_t bone_indices[MAX_BONE_INFLUENCES];
	};

	// メッシュバッファ用
	struct MeshConstantBuffer {
		XMFLOAT4X4 world;
		XMFLOAT4 material_color;
	};

	// シーンバッファ用
	struct SceneConstantBuffer {
		XMFLOAT4X4 view_projection;
		XMFLOAT4 light_direction;
		XMFLOAT4 camera_position;
	};

	// マテリアル
	struct Material {
		// ID
		uint64_t unique_id;
		// 名前
		std::string name;
		
		// ADS
		XMFLOAT4 Ka = {0.2f,0.2f,0.2f,1.0f};	// Albedo
		XMFLOAT4 Kd = {0.8f,0.8f,0.8f,1.0f};	// Diffuse
		XMFLOAT4 Ks = {1.0f,1.0f,1.0f,1.0f};	// Specular
		
		// テクスチャの名前
		std::string texture_filenames[4];
		// シェーダーリソースビュー
		/*ComPtr<*/D3D12_GPU_DESCRIPTOR_HANDLE/*>*/ shader_resource_views[4];
	};
	std::unordered_map<uint64_t, Material> materials;
	
	// メッシュ構造体
	struct Mesh
	{
		// メッシュID
		uint64_t unique_id = 0;
		// メッシュ名
		std::string name;
		int64_t node_index = 0;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		// デフォルトのグローバルトランスフォーム
		XMFLOAT4X4 default_grlobal_transform = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};

		// 複数マテリアル
		struct Subset {
			// マテリアルID
			uint64_t material_unique_id = 0;
			// マテリアル名
			std::string material_name;
			
			// スタートインデックス
			uint32_t start_index = 0;
			// インデックスカウント
			uint32_t index_count = 0;

			// メッシュ定数バッファ
			ComPtr<ID3D12Resource> mesh_constant_buffer_;
		};
		std::vector<Subset> subsets;

	private:
		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexBuffer;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
		// インデックスバッファ
		ComPtr<ID3D12Resource> indexBuffer;
		// インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW ibView = {};

		// フレンドクラス
		friend class SkinnedMesh;

	};
	std::vector<Mesh> meshes;


public:
	// Fbx用の型をXMにコンバート
	inline XMFLOAT4X4 ConvertXMFLOAT4X4FromFbx(const FbxMatrix &fbx_matrix);
	inline XMFLOAT3 ConvertXMFLOAT3FromFbx(const FbxDouble3 &fbx_double3);
	inline XMFLOAT4 ConvertXMFLOAT4FromFbx(const FbxDouble4 &fbx_double4);

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
	void FetchMaterial(FbxScene *fbx_scene, std::unordered_map<uint64_t, Material> &materials);

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
	void Render(ID3D12Device *dev, ComPtr<ID3D12GraphicsCommandList> cmdList, const XMFLOAT4X4 &world, const XMFLOAT4 &materialColor);

private:
	void CreateDummyMaterial(std::unordered_map<uint64_t, Material> &materials);

private:
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature_;
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate_;
	
	// シーン定数バッファ
	ComPtr<ID3D12Resource> scene_constant_buffer_;

	// シーンビュー
	Scene scene_view_;

	/// <summary>
	/// 上軸
	/// </summary>
	int axis_up_;

	/// <summary>
	/// 座標系
	/// </summary>
	int axis_coord_;

	// 座標系変換用
	const XMFLOAT4X4 coordinate_system_transforms[4] = {
		{	-1, 0, 0, 0,
			 0, 1, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1
		}, // 0:RHS Y-UP

		{	1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		}, // 1:LHS Y-UP

		{	-1, 0,  0, 0,
			 0, 0, -1, 0,
			 0, 1,  0, 0,
			 0, 0,  0, 1
		}, // 2:RHS Z-UP

		{	1, 0, 0, 0,
			0, 0, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 1
		}, // 3:LHS Z-UP
	};
};

