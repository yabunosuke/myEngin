#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

#include <vector>


class Mesh
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
	// �T�u�N���X

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


	};



private:
	// ���b�V�����
	NodeId					id = 0;
	std::vector<Vertex>		vertices;				// ���_
	std::vector<uint32_t>	indices;				// �C���f�b�N�X
	//std::vector<Subset>		subsets;				// �T�u�Z�b�g

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
};

