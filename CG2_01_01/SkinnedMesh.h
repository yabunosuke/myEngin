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
	// �m�[�h
	struct Node {
		// �m�[�hID
		uint64_t uniqueID = 0;
		// �m�[�h��
		std::string name;
		// �m�[�h�^�C�v
		FbxNodeAttribute::EType atribute = FbxNodeAttribute::EType::eUnknown;
		// �e��ID
		int64_t pearentIndex = -1;
	};

	// �m�[�h�R���e�i
	std::vector<Node> nodes;

	/// <summary>
	/// �m�[�hID�̌���
	/// </summary>
	/// <param name="uniqueID"></param>
	/// <returns>index</returns>
	int64_t indexof(uint64_t uniqueID) const {
		int64_t index = 0;
		for (const Node &node : nodes) {
			//ID�����v������index��Ԃ�
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
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;


public:	// �T�u�N���X
	// ���_�f�[�^�\����
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT3 texcoord;
	};

	// ���b�V���o�b�t�@�p
	struct MeshConstantBuffer {
		XMFLOAT4X4 world;
		XMFLOAT4 materialColor;
	};

	// �V�[���o�b�t�@�p
	struct SceneConstantBuffer {
		XMFLOAT4X4 view_projection;
		XMFLOAT4 light_direction;
		XMFLOAT4 camera_position;
	};

	//// �}�e���A��
	//struct Material {
	//	// ID
	//	uint64_t unique_id;
	//	// ���O
	//	std::string name;
	//	
	//	XMFLOAT4 Ka = {0.2f,0.2f,0.2f,1.0f};	// Albedo
	//	XMFLOAT4 Kd = {0.8f,0.8f,0.8f,1.0f};	// Diffuse
	//	XMFLOAT4 Ks = {1.0f,1.0f,1.0f,1.0f};	// Specular
	//	
	//	// �e�N�X�`���̖��O
	//	std::string texture_filenames[4];
	//	// �V�F�[�_�[���\�[�X�r���[
	//	ComPtr<ID3D12DescriptorHeap> shader_resource_views[4];
	//};
	//std::unordered_map<uint64_t, Material> materials;
	
	struct Mesh
	{
		//���b�V��ID
		uint64_t uniqueID = 0;
		// ���b�V����
		std::string name;
		int64_t nodeIndex = 0;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

	private:
		// ���_�o�b�t�@
		ComPtr<ID3D12Resource> vertexBuffer;
		// ���_�o�b�t�@�r���[
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
		// �C���f�b�N�X�o�b�t�@
		ComPtr<ID3D12Resource> indexBuffer;
		// �C���f�b�N�X�o�b�t�@�r���[
		D3D12_INDEX_BUFFER_VIEW ibView = {};


		friend class SkinnedMesh;

	};
	std::vector<Mesh> meshes;


public:
	static void CreatePipline();


public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="fileName">���\�[�X�̃t�@�C����</param>
	/// <param name="trianglate">�O�p�`���̗L��</param>
	SkinnedMesh(
		ID3D12Device *dev,
		const char *fileName,
		bool trianglate = true);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SkinnedMesh() = default;

	/// <summary>
	/// �m�[�h���烁�b�V�������擾
	/// </summary>
	/// <param name="fbxScene">�V�[��</param>
	/// <param name="meshes">���b�V���̊i�[��</param>
	void FetchMeshes(FbxScene *fbx_scene, std::vector<Mesh> &meshes);

	/// <summary>
	/// material�̎擾
	/// </summary>
	/// <param name="fbx_scene">�V�[��</param>
	/// <param name="materials">material�̊i�[��</param>
	//void FetchMaterial(FbxScene *fbx_scene, std::unordered_map<uint64_t, Material> &materials);

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="fileName">�t�@�C����</param>
	void CreateComObjects(ID3D12Device *dev, const char *fileName);

	/// <summary>
	/// �`��R�}���h���s
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="world">���[���h�s��</param>
	/// <param name="materialColor">�}�e���A���J���[</param>
	void Render(ComPtr<ID3D12GraphicsCommandList> cmdList, const XMFLOAT4X4 &world, const XMFLOAT4 &materialColor);


private:
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsignature_;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelinestate_;
	// ���b�V���萔�o�b�t�@
	ComPtr<ID3D12Resource> mesh_constant_buffer_;
	// �V�[���萔�o�b�t�@
	ComPtr<ID3D12Resource> scene_constant_buffer_;


protected:
	// �V�[���r���[
	Scene scene_view_;
};

