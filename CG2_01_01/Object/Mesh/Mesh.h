#pragma once
#include "Object/Object.h"

#include <DirectXMath.h>

#include <vector>
#include <wrl.h>

class Mesh : public Object
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
	//	  �R���X�g���N�^
	//
	//===========================================
	Mesh();

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


private:
	
	
	// ���_���
	std::vector<Vertex> vertices_;
	// ���b�V���R���e�i
	static std::vector<Mesh> meshs_;
};

