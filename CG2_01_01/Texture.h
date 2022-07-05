#pragma once
#include <d3dx12.h>
#include <wrl.h>
#include <vector>
#include <string>

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
	static HRESULT LoadTextureFromFile(ID3D12Device *dev,const wchar_t *filename,
		ID3D12DescriptorHeap **shader_resource_view);

public:
	// �e�N�X�`���̍ő吔
	static const int srv_count_ = 512;
	// �e�N�X�`���o�b�t�@
	static std::vector<ComPtr<ID3D12Resource>> texture_buffer_;
	// �V�F�[�_�[���\�[�X
	static ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
	
};

