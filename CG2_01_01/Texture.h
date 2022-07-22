#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include <vector>
#include <DirectXMath.h>

class Texture
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	/// <summary>
	/// �f�X�N���v�^�𐶐�
	/// </summary>
	static void CreateDescriptorHeap(ID3D12Device *dev);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="filename">�t�@�C����</param>
	/// <param name="shader_resource_view"></param>
	/// <returns></returns>
	static int LoadTextureFromFile(ID3D12Device *dev,const wchar_t *filename);

	/// <summary>
	/// �P�F�_�~�[�e�N�X�`������
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="color">�F AABBGGRR</param>
	/// <param name="dimension"></param>
	/// <returns></returns>
	static int MakeTexture(ID3D12Device *dev, DWORD color = 0xffffffff, DirectX::XMFLOAT2 size ={8,8});
	
	// �e�N�X�`���̍ő吔
	static const int srv_count_ = 512;
	// �V�F�[�_�[���\�[�X
	static ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
	// �e�N�X�`���o�b�t�@
	static std::vector<ComPtr<ID3D12Resource>> texture_buffer_;


	
};

