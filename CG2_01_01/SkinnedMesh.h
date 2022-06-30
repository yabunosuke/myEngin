#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <fbxsdk.h>

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
	
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT3 texcoord;
	};

	// �萔
	struct Constants {
		XMFLOAT4X4 world;
		XMFLOAT4 materialColor;
	};

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

private:

	// �萔�o�b�t�@
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

