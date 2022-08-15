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


// DirectXMath�\���̗pTemplate
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
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;
	// ID��UINT64���g�p����
	using NodeId = uint64_t;


public:
	// �V���A���C�Y���̊g���q
	const char *cerealize_extension = "model_v2_1";

	// �ő�ǂݍ��݃{�[����
	static const int MAX_BONES = 256;
	struct MeshConstantBuffer {
		XMFLOAT4X4 bone_transforms[MAX_BONES] = { {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		} };
	};

	// �T�u�Z�b�g�o�b�t�@
	struct SubsetConstantBuffer {
		XMFLOAT4 material_color = { 1,1,1,1 };
	};


	// �e�N�X�`���̎��
	enum class TextureType {
		BASE,
		NORMAL,
		MS,
		MAX
	};

	// �e�N�X�`����
	const std::string filename[static_cast<int>(TextureType::MAX)]{
		"",			// �x�[�X
		"",			// �m�[�}���}�b�v
		"_MS"		// ���^���b�N�ƃX���[�X�l�X
	};

	// �m�[�h
	struct Node {
		NodeId					id = 0;				// �m�[�hID
		std::string				name;				// �m�[�h��
		FbxNodeAttribute::EType	attribute = FbxNodeAttribute::EType::eUnknown;	// �m�[�h�^�C�v
		int64_t					parent_index = -1;	// �e�̃C���f�b�N�X

		// �g�����X�t�H�[��
		DirectX::XMFLOAT3 scaling = { 1,1,1 };
		DirectX::XMFLOAT4 rotation = { 0,0,0,1 };
		DirectX::XMFLOAT3 translation = { 0,0,0 };

		template < class T>
		void serialize(T &archive) {
			archive(id, name, attribute, parent_index, scaling, rotation, translation);
		}
	};

	// �}�e���A��
	struct Material {
		NodeId					id = 0;					// �m�[�hID
		std::string				name;					// �m�[�h��
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

	// �T�u�Z�b�g
	struct Subset {
		uint32_t				start_index = 0;		// �X�^�[�g�C���f�b�N�X
		uint32_t				index_count = 0;		// �C���f�b�N�X�J�E���g
		uint32_t				material_index = 0;		// �}�e���A���C���f�b�N�X

		Material *material = nullptr;

		// �T�u�Z�b�g�萔�o�b�t�@
		ComPtr<ID3D12Resource> subset_constant_buffer_;

		template < class T>
		void serialize(T &archive) {
			archive(start_index, index_count, material_index);
		}
	};

	// �e�����󂯂�{�[����
	static const int MAX_BONE_INFLUENCES = 4;
	// ���_�f�[�^�\����
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

	// ���b�V���\����
	struct Mesh
	{
		// ���b�V�����
		NodeId					id = 0;
		std::vector<Vertex>		vertices;				// ���_
		std::vector<uint32_t>	indices;				// �C���f�b�N�X
		std::vector<Subset>		subsets;				// �T�u�Z�b�g

		// �{�[�����
		std::vector<uint32_t>	node_indices;			// �{�[��
		std::vector<XMFLOAT4X4>	offset_transforms;		// ?


		// ���_�o�b�t�@
		ComPtr<ID3D12Resource> vertexBuffer;
		// ���_�o�b�t�@�r���[
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
		// �C���f�b�N�X�o�b�t�@
		ComPtr<ID3D12Resource> indexBuffer;
		// �C���f�b�N�X�o�b�t�@�r���[
		D3D12_INDEX_BUFFER_VIEW ibView = {};

		// ���b�V���萔�o�b�t�@
		ComPtr<ID3D12Resource> mesh_constant_buffer_;

		/// <summary>
		/// �{�[���̌���
		/// </summary>
		/// <param name="uniqueID"></param>
		/// <returns>index</returns>
		NodeId indexof(NodeId unique_id) const {
			int64_t index = 0;
			for (const int &ix : node_indices) {
				//ID�����v������index��Ԃ�
				if (ix == unique_id) {
					return index;
				}
				++index;
			}
			return -1;
		}

		// �g���p�C�v���C���̖��O
		std::string pipline_name = "GBuffer";

		template < class T>
		void serialize(T &archive) {
			archive(id, vertices, indices, subsets, node_indices, offset_transforms);
		}
	};

	// �|�[�Y�f�[�^
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

	// �L�[�t���[��
	struct Keyframe
	{
		float						seconds; //�L�[�t���[���̊J�n����
		std::vector<NodeKeyData>	nodeKeys;

		template<class T>
		void serialize(T &archive)
		{
			archive(seconds, nodeKeys);
		}
	};

	// �A�j���[�V����
	struct Animation
	{
		std::string					name;			// �A�j���[�V������
		float						seconds_length;	// �A�j���[�V�����ɂ����鎞��
		float						sampling_rate;	// �T���v�����O���[�g
		std::vector<Keyframe>		keyframes;		// �L�[�t���[���R���e�i

		template<class T>
		void serialize(T &archive)
		{
			archive(name, seconds_length, sampling_rate, keyframes);
		}
	};

public: //�ÓI

	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	void LoadModel(ID3D12Device *dev, const char *file_name);

public:
	// �Q�b�^�[
	const std::vector<Mesh> &GetMeshes() const { return meshes_; }
	const std::vector<Node> &GetNodes() const { return nodes_; }
	const std::vector<Material> &GetMaterials() const { return materials_; }
	const std::vector<Animation> &GetAnimations() const { return animations_; }

private:
	/// <summary>
	/// �m�[�h�C���f�b�N�X����
	/// </summary>
	/// <param name="nodeId">�����������m�[�hID</param>
	/// <returns>�C���f�b�N�X�ԍ�</returns>
	NodeId FindNodeIndex(NodeId nodeId) const;

	/// <summary>
	/// �m�[�h���烁�b�V�������擾
	/// </summary>
	/// <param name="fbxScene">�V�[��</param>
	/// <param name="meshes">���b�V���̊i�[��</param>
	void FetchMeshes(FbxScene *fbx_scene, std::vector<Mesh> &meshes);

	/// <summary>
	/// �o�C���h�|�[�Y�̎擾
	/// </summary>
	/// <param name="fbx_mesh">�V�[��</param>
	/// <param name="bind_pose"></param>
	void FetchSkeleton(FbxMesh *fbx_mesh, Mesh *bind_pose);

	/// <summary>
	/// material�̎擾
	/// </summary>
	/// <param name="fbx_scene">�V�[��</param>
	/// <param name="materials">material�̊i�[��</param>
	void FetchMaterial(FbxScene *fbx_scene, std::vector<Material> &materials);


	/// <summary>
	/// �A�j���[�V�����擾
	/// </summary>
	/// <param name="fbx_scene">�V�[��</param>
	/// <param name="animation_clips"></param>
	/// <param name="sampling_rate"></param>
	void FetchAnimations(FbxScene *fbx_scene, std::vector<Animation> &animation_clips, float sampling_rate = 0);

	/// <summary>
	/// �}�e���A���ƃT�u�Z�b�g�̕R�Â�
	/// </summary>
	void BindMaterial();

	/// <summary>
	/// ComObject����
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	void CreateComObjects(ID3D12Device *dev);

private:
	std::vector<Node>		nodes_;
	std::vector<Material>	materials_;
	std::vector<Mesh>		meshes_;
	std::vector<Animation>	animations_;

	std::string				filename_;

};

