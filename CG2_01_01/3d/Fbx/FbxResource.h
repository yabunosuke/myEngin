#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>


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

class FbxResource
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
	// シリアライズ時の拡張子
	const char *cerealize_extension = "model_v2_1";

	// 最大読み込みボーン数
	static const int MAX_BONES = 256;
	struct MeshConstantBuffer {
		XMFLOAT4X4 bone_transforms[MAX_BONES] = { {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		} };
	};

	// サブセットバッファ
	struct SubsetConstantBuffer {
		XMFLOAT4 material_color = { 1,1,1,1 };
	};


	// テクスチャの種類
	enum class TextureType {
		BASE,
		NORMAL,
		MS,
		MAX
	};

	// テクスチャ名
	const std::string filename[static_cast<int>(TextureType::MAX)]{
		"",			// ベース
		"",			// ノーマルマップ
		"_MS"		// メタリックとスムースネス
	};

	// ノード
	struct Node {
		NodeId					id = 0;				// ノードID
		std::string				name;				// ノード名
		FbxNodeAttribute::EType	attribute = FbxNodeAttribute::EType::eUnknown;	// ノードタイプ
		int64_t					parent_index = -1;	// 親のインデックス

		// トランスフォーム
		DirectX::XMFLOAT3 scaling = { 1,1,1 };
		DirectX::XMFLOAT4 rotation = { 0,0,0,1 };
		DirectX::XMFLOAT3 translation = { 0,0,0 };

		template < class T>
		void serialize(T &archive) {
			archive(id, name, attribute, parent_index, scaling, rotation, translation);
		}
	};

	// マテリアル
	struct Material {
		NodeId					id = 0;					// ノードID
		std::string				name;					// ノード名
		/// <summary>
		///	<para>0. BaseTexture </para>
		///	<para>1. NormalTexture </para>
		/// </summary>
		std::string				texture_filenames[2];

		DirectX::XMFLOAT4		color = { 0.8f,0.8f,0.8f,1.0f };

		int						texture_id;
		D3D12_GPU_DESCRIPTOR_HANDLE shader_resource_views[static_cast<int>(TextureType::MAX)];

		template < class T>
		void serialize(T &archive) {
			archive(id, name, texture_filenames, color);
		}
	};

	// サブセット
	struct Subset {
		uint32_t				start_index = 0;		// スタートインデックス
		uint32_t				index_count = 0;		// インデックスカウント
		uint32_t				material_index = 0;		// マテリアルインデックス

		Material *material = nullptr;

		// サブセット定数バッファ
		ComPtr<ID3D12Resource> subset_constant_buffer_;

		template < class T>
		void serialize(T &archive) {
			archive(start_index, index_count, material_index);
		}
	};

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


		template < class T>
		void serialize(T &archive) {
			archive(position, normal, tangent, texcoord, color, bone_weights, bone_indices);
		}
	};

	// メッシュ構造体
	struct Mesh
	{
		// メッシュ情報
		NodeId					id = 0;
		std::vector<Vertex>		vertices;				// 頂点
		std::vector<uint32_t>	indices;				// インデックス
		std::vector<Subset>		subsets;				// サブセット

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

		/// <summary>
		/// ボーンの検索
		/// </summary>
		/// <param name="uniqueID"></param>
		/// <returns>index</returns>
		NodeId indexof(NodeId unique_id) const {
			int64_t index = 0;
			for (const int &ix : node_indices) {
				//IDが合致したらindexを返す
				if (ix == unique_id) {
					return index;
				}
				++index;
			}
			return -1;
		}

		// 使うパイプラインの名前
		std::string pipline_name = "GBuffer";

		template < class T>
		void serialize(T &archive) {
			archive(id, vertices, indices, subsets, node_indices, offset_transforms);
		}
	};

	// ポーズデータ
	struct NodeKeyData
	{
		XMFLOAT3				scale;
		XMFLOAT4				rotate;
		XMFLOAT3				translate;
		template<class T>
		void serialize(T &archive)
		{
			archive(scale, rotate, translate);
		}
	};

	// キーフレーム
	struct Keyframe
	{
		float						seconds; //キーフレームの開始時間
		std::vector<NodeKeyData>	nodeKeys;

		template<class T>
		void serialize(T &archive)
		{
			archive(seconds, nodeKeys);
		}
	};

	// アニメーション
	struct Animation
	{
		std::string					name;			// アニメーション名
		float						seconds_length;	// アニメーションにかかる時間
		float						sampling_rate;	// サンプリングレート
		std::vector<Keyframe>		keyframes;		// キーフレームコンテナ

		template<class T>
		void serialize(T &archive)
		{
			archive(name, seconds_length, sampling_rate, keyframes);
		}
	};

public: //静的

	/// <summary>
	/// モデル読み込み
	/// </summary>
	void LoadModel(ID3D12Device *dev, const char *file_name);

public:
	// ゲッター
	const std::vector<Mesh> &GetMeshes() const { return meshes_; }
	const std::vector<Node> &GetNodes() const { return nodes_; }
	const std::vector<Material> &GetMaterials() const { return materials_; }
	const std::vector<Animation> &GetAnimations() const { return animations_; }

private:
	/// <summary>
	/// ノードインデックス検索
	/// </summary>
	/// <param name="nodeId">検索したいノードID</param>
	/// <returns>インデックス番号</returns>
	NodeId FindNodeIndex(NodeId nodeId) const;

	/// <summary>
	/// ノードからメッシュ情報を取得
	/// </summary>
	/// <param name="fbxScene">シーン</param>
	/// <param name="meshes">メッシュの格納先</param>
	void FetchMeshes(FbxScene *fbx_scene, std::vector<Mesh> &meshes);

	/// <summary>
	/// バインドポーズの取得
	/// </summary>
	/// <param name="fbx_mesh">シーン</param>
	/// <param name="bind_pose"></param>
	void FetchSkeleton(FbxMesh *fbx_mesh, Mesh *bind_pose);

	/// <summary>
	/// materialの取得
	/// </summary>
	/// <param name="fbx_scene">シーン</param>
	/// <param name="materials">materialの格納先</param>
	void FetchMaterial(FbxScene *fbx_scene, std::vector<Material> &materials);


	/// <summary>
	/// アニメーション取得
	/// </summary>
	/// <param name="fbx_scene">シーン</param>
	/// <param name="animation_clips"></param>
	/// <param name="sampling_rate"></param>
	void FetchAnimations(FbxScene *fbx_scene, std::vector<Animation> &animation_clips, float sampling_rate = 0);

	/// <summary>
	/// マテリアルとサブセットの紐づけ
	/// </summary>
	void BindMaterial();

	/// <summary>
	/// ComObject生成
	/// </summary>
	/// <param name="dev">デバイス</param>
	void CreateComObjects(ID3D12Device *dev);

private:
	std::vector<Node>		nodes_;
	std::vector<Material>	materials_;
	std::vector<Mesh>		meshes_;
	std::vector<Animation>	animations_;

	std::string				filename_;

};

