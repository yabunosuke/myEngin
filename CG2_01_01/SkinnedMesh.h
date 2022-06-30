#pragma once
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
	// ���_�f�[�^�\����
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT3 texcoord;
	};

	// �萔�o�b�t�@�p
	struct Constants {
		XMFLOAT4X4 world;
		XMFLOAT4 materialColor;
	};

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
		bool trianglate = false);
	virtual ~SkinnedMesh() = default;

private:
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelinestate;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;

protected:
	// �V�[���r���[
	Scene sceneView;
};

