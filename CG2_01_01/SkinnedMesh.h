#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <fbxsdk.h>
#include <unordered_map>

//シリアライズ
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/unordered_map.hpp>

//#include "Component.h"

// DirectXMath構造体用Template
namespace DirectX {
	template<class T>
	void serialize(T &archive, DirectX::XMFLOAT2 &v) {
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y)
		);
	}

	template<class T>
	void serialize(T &archive, DirectX::XMFLOAT3 &v) {
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z)
		);
	}

	template < class T>
	void serialize(T &archive, DirectX::XMFLOAT4 &v) {
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z),
			cereal::make_nvp("w", v.w)
		);
	}

	template < class T>
	void serialize(T &archive, DirectX::XMFLOAT4X4 &m) {
		archive(
			cereal::make_nvp("_11", m._11), cereal::make_nvp("_12", m._12),
			cereal::make_nvp("_13", m._13), cereal::make_nvp("_14", m._14),
			cereal::make_nvp("_21", m._21), cereal::make_nvp("_22", m._22),
			cereal::make_nvp("_23", m._23), cereal::make_nvp("_24", m._24),
			cereal::make_nvp("_31", m._31), cereal::make_nvp("_32", m._32),
			cereal::make_nvp("_33", m._33), cereal::make_nvp("_34", m._34),
			cereal::make_nvp("_41", m._41), cereal::make_nvp("_42", m._42),
			cereal::make_nvp("_43", m._43), cereal::make_nvp("_44", m._44)
		);
	}

}


struct Scene {
	// ノード
	struct Node {
		// ノードID
		uint64_t unique_id = 0;
		// ノード名
		std::string name;
		// ノードタイプ
		FbxNodeAttribute::EType atribute = FbxNodeAttribute::EType::eUnknown;
		// 親のID
		int64_t parent_index = -1;

		template < class T>
		void serialize(T &archive) {
			archive(unique_id, name, atribute, parent_index);
		}
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
			if (node.unique_id == uniqueID) {
				return index;
			}
			index++;
		}
		return -1;
	}

	template < class T>
	void serialize(T &archive) {
		archive(nodes);
	}
};

struct Skeleton {
	struct Bone {
		uint64_t unique_id = 0;
		std::string name;

		int64_t parent_index = -1;
		int64_t node_index = 0;

		DirectX::XMFLOAT4X4 offset_transform = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};

		/// <summary>
		/// 親がいないか
		/// </summary>
		/// <returns>親がいなければtrue</returns>
		bool is_orphan() const { return parent_index < 0; };

		template < class T>
		void serialize(T & archive) {
			archive(unique_id, name, parent_index, node_index, offset_transform);
		}
	};
	std::vector<Bone> bones;

	int64_t indexof(uint64_t unique_id) const {
		int64_t index = 0;
		for (const Bone &bone : bones) {
			if (bone.unique_id == unique_id) {
				return index;
			}
			++index;
		}
		return -1;
	}

	template < class T>
	void serialize(T &archive) {
		archive(bones);
	}
};

struct Animation {
	std::string name;
	float sampling_rate = 0;

	struct Keyframe {
		struct Node {
			DirectX::XMFLOAT4X4 global_transform = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
			};

			DirectX::XMFLOAT3 scaling = { 1,1,1 };
			DirectX::XMFLOAT4 rotation = { 0,0,0,1 };
			DirectX::XMFLOAT3 translation = { 0,0,0 };

			template < class T>
			void serialize(T & archive) {
				archive(global_transform, scaling, rotation, translation);
			}
		};
		std::vector<Node> nodes;


		template < class T>
		void serialize(T &archive) {
			archive(nodes);
		}
	};
	// シークエンス
	std::vector<Keyframe> sequence;


	template < class T>
	void serialize(T &archive) {
		archive(name, sampling_rate, sequence);
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


		template < class T>
		void serialize(T &archive) {
			archive(position, normal, texcoord, bone_weights, bone_indices);
		}
	};

	// 最大読み込みボーン数
	static const int MAX_BONES = 256;
	// メッシュバッファ用
	struct MeshConstantBuffer {
		XMFLOAT4X4 world = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
		XMFLOAT4 material_color;
		XMFLOAT4X4 bone_transforms[MAX_BONES] = { {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		} };
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

		template < class T>
		void serialize(T & archive) {
			archive(unique_id, name, Ka, Kd, Ks, texture_filenames);
		}
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


			template < class T>
			void serialize(T &archive) {
				archive(material_unique_id, material_name, start_index, index_count);
			}
		};
		std::vector<Subset> subsets;


		template < class T>
		void serialize(T &archive) {
			archive(unique_id, node_index, subsets, default_grlobal_transform, bind_pose, vertices, indices);
		}
		
	private:
		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexBuffer;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
		// インデックスバッファ
		ComPtr<ID3D12Resource> indexBuffer;
		// インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW ibView = {};

		// バインドポーズ
		Skeleton bind_pose;

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
	/// バインドポーズの取得
	/// </summary>
	/// <param name="fbx_mesh">シーン</param>
	/// <param name="bind_pose"></param>
	void FetchSkeleton(FbxMesh *fbx_mesh, Skeleton &bind_pose);

	/// <summary>
	/// アニメーション取得
	/// </summary>
	/// <param name="fbx_scene">シーン</param>
	/// <param name="animation_clips"></param>
	/// <param name="sampling_rate"></param>
	void FetchAnimations(FbxScene *fbx_scene, std::vector<Animation> &animation_clips, float sampling_rate = 0);


	void UpdateAnimation(Animation::Keyframe &keyframe);

	bool AppendAnimations(const char *animation_filename, float sampling_rate = 0.0f);

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
	void Render(ID3D12Device *dev, ComPtr<ID3D12GraphicsCommandList> cmdList, const XMFLOAT4X4 &world, const XMFLOAT4 &materialColor,const Animation::Keyframe *keyframe);

public:
	std::vector<Animation> animation_clips_;

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

	//
	float scale_factor_;
};

