#pragma once
#include "Object/Object.h"

#include <DirectXMath.h>
#include "Math/Vector3.h"

#include <vector>
#include <wrl.h>

// �V���A���C�Y�p
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

class Mesh :
	public Object
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
	//===========================================
	//
	//		�T�u�N���X
	//
	//===========================================
	struct BoneWeight {
		// �C���f�b�N�X
		int bone_index_0;
		int bone_index_1;
		int bone_index_2;
		int bone_index_3;

		// �E�F�C�g
		float weight0;
		float weight1;
		float weight2;
		float weight3;
	};

	// �e�����󂯂�{�[����
	static const int MAX_BONE_INFLUENCES = 4;
	struct Vertex // ���_���
	{
		XMFLOAT4				position = { 0, 0, 0, 1 };		// ���W
		XMFLOAT3				normal = { 0, 0, 0 };			// �@��
		XMFLOAT3				tangent = { 0, 0, 0 };			// �ڐ�
		XMFLOAT3				texcoord = { 0, 0 ,0 };			// �F���
		XMFLOAT4				color = { 1, 1, 1, 1 };
		float bone_weights[MAX_BONE_INFLUENCES] = { 1,0,0,0 };
		uint32_t bone_indices[MAX_BONE_INFLUENCES];
	};


	//===========================================
	//
	//		�R���X�g���N�^
	//
	//===========================================
	Mesh();


	//===========================================
	//
	//		�����o�֐�
	//
	//===========================================


	//===========================================
	//
	//		�ÓI�֐�
	//
	//===========================================


	//===========================================
	//
	//		�A�N�Z�b�T
	//
	//===========================================

	
private:
	
	DirectX::XMMATRIX bindpose_;	// �o�C���h�|�[�Y
	
	// ���_���
	std::vector<Vector3> vertices_;		// ���_
	std::vector<uint32_t> indices_;		// �C���f�b�N�X

	std::vector<Vector3> normals_;		// ���b�V���̖@��
	std::vector<Vector3> tangents_;		// ���b�V���̐ڐ�


};

