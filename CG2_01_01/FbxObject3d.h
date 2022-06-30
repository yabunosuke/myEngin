#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include "SkinnedMesh.h"
class FbxObject3d
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	FbxObject3d(SkinnedMesh *skinnedMesh);

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	void CreateGraphicsPipeline(ID3D12Device *dev);
	// �`��
	void Draw(ComPtr<ID3D12GraphicsCommandList> cmdList) const;
private:
	//���b�V���f�[�^
	SkinnedMesh *mesh;
	
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelinestate;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsignature;

};

